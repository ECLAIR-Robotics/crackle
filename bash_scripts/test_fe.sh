#!/bin/bash

BASE=http://localhost:8137
T=demo-1

# --- IDLE: a few pings spaced out, like the real FSM idles between wake
# checks. The pipeline view's Idle node should glow green once a couple of
# these have landed in a row (see PIPELINE_UI_ARCHITECTURE / pipeline.html's
# "healthy" state), not just on the first one. ---
curl -s $BASE/update -d '{"state":"idle"}'
sleep 2
curl -s $BASE/update -d '{"state":"idle"}'
sleep 2
curl -s $BASE/update -d '{"state":"idle"}'
sleep 2

# --- LISTENING: paced slowly enough to actually see the Input -> Whisper AI
# -> Output pipeline advance, instead of blowing past it in under a second.
# No turn_id here — main.py's real api:whisper_stt instrumentation doesn't
# attach one (see PIPELINE_API_CONTRACT.md §5); attaching one here was
# accidentally triggering an early switch to the TASK view. ---
curl -s $BASE/update -d '{"state":"listening","wakeword":true}'
sleep 2.5
curl -s $BASE/pipeline -d '{"stage":"api:whisper_stt","status":"loading"}'
sleep 2.5
curl -s $BASE/pipeline -d '{"stage":"api:whisper_stt","status":"done","payload":{"text":"grab the mug, put it down on the table, then wave"}}'
sleep 2.5

# --- TASK: Plan node ---
curl -s $BASE/update -d '{"state":"thinking"}'
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"llm_plan\",\"status\":\"start\",\"payload\":{\"prompt\":\"grab the mug, put it down on the table, then wave\"}}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"loading\"}"
sleep 1.5
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"done\"}"
sleep 1
# First plan — its first step will fail below.
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"llm_plan\",\"status\":\"data\",\"payload\":{\"preamble\":\"You betcha, on it!\",\"steps\":[\"pick_up(coffee_mug) — user requested\",\"place() — set it down on the table\",\"wave() — say goodbye\"]}}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:elevenlabs_tts\",\"status\":\"loading\",\"payload\":{\"node\":\"plan\",\"text\":\"You betcha, on it!\"}}"
sleep 0.8
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:elevenlabs_tts\",\"status\":\"done\",\"payload\":{\"node\":\"plan\"}}"

# --- Execution block #1: step 0 (pick_up) FAILS — should turn red. ---
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":0,\"stage\":\"tool:pick_up\",\"status\":\"start\",\"payload\":{\"args\":{\"object_name\":\"coffee mug\"}}}"
sleep 2
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":0,\"stage\":\"tool:pick_up\",\"status\":\"error\",\"payload\":{\"success\":false,\"message\":\"Found 'coffee_mug_0' in the scene but the pick-up motion failed — I couldn't grasp it.\"}}"
sleep 1.5

# --- Back to planning: the LLM sees the failure and submits a NEW plan.
# This should create a SECOND Execution block ("Execution — replan 2"),
# not overwrite the first one's rows. ---
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"loading\"}"
sleep 1.2
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"done\"}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"llm_plan\",\"status\":\"data\",\"payload\":{\"preamble\":\"\",\"steps\":[\"pick_up(coffee_mug) — retry with a different grasp\",\"place() — set it down on the table\",\"wave() — say goodbye\"]}}"
sleep 1

# --- Execution block #2: all three steps succeed this time. ---
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":0,\"stage\":\"tool:pick_up\",\"status\":\"start\",\"payload\":{\"args\":{\"object_name\":\"coffee mug\"}}}"
sleep 1.5
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":0,\"stage\":\"tool:pick_up\",\"status\":\"done\",\"payload\":{\"success\":true,\"matched_object\":\"coffee_mug_0\"}}"
sleep 0.5
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":1,\"stage\":\"tool:place\",\"status\":\"start\",\"payload\":{\"args\":{}}}"
sleep 1.2
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":1,\"stage\":\"tool:place\",\"status\":\"done\",\"payload\":{\"success\":true}}"
sleep 0.5
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":2,\"stage\":\"tool:wave\",\"status\":\"start\",\"payload\":{\"args\":{}}}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"step_index\":2,\"stage\":\"tool:wave\",\"status\":\"done\",\"payload\":{\"success\":true}}"

# --- TASK: End node ---
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"loading\"}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:openai_responses\",\"status\":\"done\"}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"llm_plan\",\"status\":\"done\",\"payload\":{\"dialogue\":\"All set — took two tries on the mug, but it's on the table and I gave ya a wave!\",\"emotion\":\"happy\"}}"
sleep 1
# Only "end" ever gets a face event — see pipeline.html's comment on the Plan node.
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"face\",\"status\":\"data\",\"payload\":{\"node\":\"end\",\"emotion\":\"happy\"}}"
sleep 1
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:elevenlabs_tts\",\"status\":\"loading\",\"payload\":{\"node\":\"end\",\"text\":\"All set...\"}}"
sleep 1.2
curl -s $BASE/pipeline -d "{\"turn_id\":\"$T\",\"stage\":\"api:elevenlabs_tts\",\"status\":\"done\",\"payload\":{\"node\":\"end\"}}"
sleep 1

# --- back to idle — a few pings again, spaced out, mirroring the start. ---
curl -s $BASE/update -d '{"state":"idle","emotion":"happy"}'
sleep 2
curl -s $BASE/update -d '{"state":"idle"}'
sleep 2
curl -s $BASE/update -d '{"state":"idle"}'
