#!/usr/bin/env python3

import os
import hashlib

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

if __name__=="__main__":

    root_dir="files"
    catalog={}
    for f in os.listdir(root_dir):
        full_path=os.path.join(root_dir,f)
        
        if not os.path.islink(full_path):
            file_hash=md5(full_path)
            if file_hash in catalog:
                catalog[file_hash].append(full_path)
            else:
                catalog[file_hash]=[]

    for h in catalog.keys():
        print("Source file {}".format(catalog[h][0]))
        for i in range(1,len(catalog[h])):
            os.remove(catalog[h][i])
            os.symlink(catalog[h][0],catalog[h][i])