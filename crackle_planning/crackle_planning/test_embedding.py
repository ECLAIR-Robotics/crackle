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
    mapped_strings: List[str],
    target_string: str,
    all: bool,
    embeddings: List[str] = [],
    model: str = "text-embedding-3-small",
) -> List[int]:
    """Return nearest neighbors of a given string (including itself first)."""

    # embeddings for all mapped strings
    if len(embeddings) == 0:
        embeddings = [embedding_from_string(s, model=model) for s in mapped_strings]

    # embeddings for target strings
    target_embedding = embedding_from_string(target_string, model=model)

    # cosine *distance* = 1 - cosine similarity
    q_norm = np.linalg.norm(target_embedding)
    distances = []
    for e in embeddings:
        denom = q_norm * np.linalg.norm(e)
        sim = float(target_embedding @ e / denom) if denom != 0 else 0.0
        distances.append(1.0 - sim)

    # nearest neighbors = smallest distance first
    indices_of_nearest_neighbors = list(np.argsort(distances))

    if all:
        res = []
        for i in indices_of_nearest_neighbors:
            if distances[i] <= 0.52:
                res.append(mapped_strings[i])
        
        if len(res) > 0:
            return res
        return None

    closest_idx = indices_of_nearest_neighbors[0]
    if distances[closest_idx] <= 0.52:
        return mapped_strings[closest_idx]
    return None

def test_embedding():
    target_string = "cell phone"
    print("Target sting: ", target_string)

    print("\nTESTING find closest...")
    print(find(target_string))
    
    print("\nTESTING find all...")
    print(find(target_string, all=True))
    

def find(target: str, all: bool = False):
    # TODO make call to mapped objects list
    # mapped = 

    # Placeholder
    mapped = ["telephone", "puppy", "banana", "gun", "razor", "Phineas", "Ferb", "iphone", "sock", "water bottle", "coke can", "phone", "waffle fries", "robot", "fruit"] 
    
    return recommendations_from_strings(mapped, target, all)
    
if __name__ == "__main__":
    print("Starting test...")
    test_embedding()
