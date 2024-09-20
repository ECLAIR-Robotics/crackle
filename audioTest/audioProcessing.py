# Credit to ChatGPT
# (if it doesnt work --> blame chatGPT)
#
# Handles getting of microphone data and processing of microphone data
# Goal: use two microphones a known distance apart to find the angle from their center that a sound is originated
# Units / Standards:
#   Distance = mm
#   Time = ms
#   Consider left side to be positive
#
# TODO: figure out a way to find the actual of 2 angles a sound originated form
# TODO: make a method to send the data to subscribers
# TODO: find out a way to prevent timeouts in case of a mike disconnection

import pyaudio
import numpy as np
import time

# Physics constants
MIKE_DISTANCE = 0
SPEED_OF_SOUND = 343000000

# Mike constants
SAMPLE_RATE = 44100  # Sample rate in Hz
CHUNK = 1024  # Number of frames per buffer
THRESHOLD = 0.01  # Threshold for detecting sound
LEFT_MIKE_INDEX = 0
RIGHT_MIKE_INDEX = 1

# time after which timeout has probably occured: after this point reset the test
MAX_WAIT_TIME = 1000

# Opens an audio path for the given mike
def getAudioStream(mic_index):
    p = pyaudio.PyAudio()
    stream = p.open(
        format=pyaudio.paInt16,
        channels=1,
        rate=SAMPLE_RATE,
        input=True,
        input_device_index=mic_index,
        frames_per_buffer=CHUNK,
    )
    return p, stream


# captues a bit of audio and returns the highest amplitude measured
# note: stream is the stream from a particular microphone
def captureSound(stream):
    data = np.frombuffer(stream.read(CHUNK), dtype=np.int16)
    return np.max(np.abs(data))


# gets the delta between two times to find how far apart the time the first and second sound were
def getDeltaTime(leftMikeTime, rightMikeTime):
    return rightMikeTime - leftMikeTime


# gets the two possible angles from center to origination of sound using funny trig stuff
def getAngle(deltaTime):
    delta = (deltaTime * SPEED_OF_SOUND) / MIKE_DISTANCE
    angle1 = np.arctan2(delta, 1)
    angle2 = np.arctan2(-delta, 1)
    return np.degrees(angle1), np.degrees(angle2)


# checks if sound is significant / worth recording from
def significantAudio(sound):
    return sound > THRESHOLD


# return time at which second sound is recorded
# get out if waiting for too long
def secondMike(audioStream):
    tStart = time.time()  # time at which started waiting
    while tStart - time.time() < MAX_WAIT_TIME:
        if significantAudio(audioStream):
            return time.time()
    return 0


# Note: individually checks for sound from left side and right side
#   will work -->assuming<-- both mikes pick up the same sounds
#   (at a slight time delay) (which they should in basically every circumstance if
#   thereshold is properly set), but may cause errors if a mike unplugs
def main():
    # Used to fetch data from mikes
    leftPyAudio, leftStream = getAudioStream(LEFT_MIKE_INDEX)
    rightPyAudio, rightStream = getAudioStream(RIGHT_MIKE_INDEX)

    while 1 == 1:
        t0 = time.time()  # time at which segment recording starts
        t1 = 0
        t2 = 0
        angle = 0  # final angle for given turn

        # check for sound originating on left side
        leftVolume = captureSound(leftStream)
        if significantAudio(leftVolume):
            t1 = time.time() - t0  # the time after t0 that first sound was recorded
            t2 = (
                secondMike(rightStream) - t0
            )  # time after t1 that second sound was recorded

        # check for sound originating from right side
        rightVolume = captureSound(rightStream)
        if significantAudio(rightVolume):
            t1 = time.time() - t0  # the time after t0 that first sound was recorded
            t2 = (
                secondMike(leftStream) - t0
            )  # time after t1 that second sound was recorded

        # check if it timed out
        if t2 > 0:
            angles = getAngle(getDeltaTime(t1, t2))
            # NOTE: Reverse this comparison if robot points the opposite direction of whats wanted
            if rightVolume > leftVolume:
                angle = angles[0]
            else:
                angle = angles[1]
