#!/usr/bin/env python3

import os
import random


# generate n_files files, sample_size size each in root_dir
if __name__=="__main__":
    n_samples=20
    sample_size=20480 #bytes
    n_files=20000
    root_dir="files"

    samples=[]
    for i in range(n_samples):
        samples.append(os.urandom(sample_size))

    # create files in files directory
    if not os.path.exists(root_dir):
        os.mkdir(root_dir)
    for i in range(n_files):
        random_sample_idx=random.randint(0,n_samples-1)
        with open(os.path.join(root_dir,"file.{}".format(i)), "wb") as f:
            f.write(samples[random_sample_idx])
