from dotenv import load_dotenv
from elevenlabs.client import ElevenLabs
from elevenlabs.play import play
import os
from crackle_planning._llm import GptAPI
from crackle_planning._keys import openai_key, elevenlabs_key 
"""
Reference API at https://elevenlabs.io/docs/quickstart

Note: Pip installed
    Elevenlabs
    python-dotenv
"""
elevenlabs = ElevenLabs(
  api_key=elevenlabs_key
)



def say_text(text):
  audio = elevenlabs.text_to_speech.convert(
    text=text,
    voice_id="Upbl8et4ENZIZ6UV3DTR",
    model_id="eleven_multilingual_v2",
    output_format="mp3_44100_128",
  )

  play(audio)
  print("Finished Speaking")
  
def proof_of_concept(prompt):
  api = GptAPI(openai_key)
  text = api.getTalkBack(prompt)
  say_text(text)
  
if __name__ == '__main__':
  proof_of_concept("Hello world!")
  