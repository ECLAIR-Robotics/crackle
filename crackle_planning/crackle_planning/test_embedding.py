import os
from typing import List
import numpy as np
import openai
from openai import OpenAI
from _keys import openai_key

os.environ["OPENAI_API_KEY"] = str(openai_key)
client = OpenAI(api_key=os.environ["OPENAI_API_KEY"])

def embedding_from_string(text: str, model: str = "text-embedding-3-small") -> np.ndarray:
    """Get an embedding as a NumPy vector."""
    resp = client.embeddings.create(model=model, input=text)
    # print("str: ", text, " | embedding: ", np.array(resp.data[0].embedding, dtype=np.float32))
    return np.array(resp.data[0].embedding, dtype=np.float32)


def recommendations_from_strings(
    strings: List[str],
    index_of_source_string: int,
    model: str = "text-embedding-3-small",
) -> List[int]:
    """Return nearest neighbors of a given string (including itself first)."""

    # 1) embeddings for all strings
    embeddings = [embedding_from_string(s, model=model) for s in strings]

    # 2) source embedding
    query = embeddings[index_of_source_string]

    # 3) cosine *distance* = 1 - cosine similarity
    q_norm = np.linalg.norm(query)
    distances = []
    for e in embeddings:
        denom = q_norm * np.linalg.norm(e)
        sim = float(query @ e / denom) if denom != 0 else 0.0
        distances.append(1.0 - sim)

    # 4) nearest neighbors = smallest distance first
    indices_of_nearest_neighbors = list(np.argsort(distances))

    closest_idx = indices_of_nearest_neighbors[1]
    if distances[closest_idx] <= 0.5:
        return strings[closest_idx]
    return None

def test_embedding():
    strings = ["cell phone", "puppy", "banana", "gun", "razor", "Phineas", "Ferb", "sock", "water bottle", "coke can", "phone", "waffle fries", "robot"]
    res = recommendations_from_strings(strings, 5)
    print(res)

if __name__ == "__main__":
    print("Starting test...")
    test_embedding()
