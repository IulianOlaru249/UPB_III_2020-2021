import random
from concurrent.futures import ThreadPoolExecutor as ThreadPool

MAX_WORKERS = 10
SAMPLE_SIZE = 1000
SEQ_LEN = 2000

def generateSamples():
    choice_str = 'ATGC'
    result = []
    random.seed(1)
    for i in range(SAMPLE_SIZE):
        current_str = ""
        for j in range(SEQ_LEN):
            current_str += random.choice(choice_str)
        result.append(current_str)
    return result

def findSeq(samples, sequence, idx):
    return sequence in samples[idx]

def threadFun(samples, sequence, idx):
    if findSeq(samples, sequence, idx):
        print(f"DNA found at idx {idx}")
    

def main():
    pool = ThreadPool(MAX_WORKERS)
    tasks = []
    search_str = "GCACTTAAAG"
    samples = generateSamples()
    sample_size = len(samples)

    with pool:
        for i in range(sample_size):
            tasks.append(pool.submit(threadFun, samples, search_str, i))

if __name__ == "__main__":
    main()
