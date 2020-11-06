import argparse
import diffimg
import json
import PIL.Image as Image
import PIL.ImageChops as ImageChops
import PIL.ImageOps as ImageOps
import os
import sys

#import xml.etree as etree
#import xml.etree.ElementTree as ET

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_dir1")
    parser.add_argument("input_dir2")
    parser.add_argument("output_dir")
    args = parser.parse_args()

    path_in1 = args.input_dir1
    path_in2 = args.input_dir2
    path_out = args.output_dir
    for item1 in os.listdir(path_in1):
        if not(os.path.isdir(os.path.join(path_in1, item1))): continue

        # create the output directory if necessary
        if not(os.path.isdir(os.path.join(path_out, item1))):
            os.mkdir(os.path.join(path_out, item1))
        
        for item2 in os.listdir(os.path.join(path_in1, item1)):
            # skip directories
            if not(os.path.isfile(os.path.join(path_in1, item1, item2))): continue
            # skip hidden files and not SVG files
            if item2.startswith('.') or not(item2.endswith('.svg')): continue

            # filenames (input SVG and output PNG)
            name, ext = os.path.splitext(item2)
            pngFile1 = os.path.join(path_in1, item1, name + '.png')
            pngFile2 = os.path.join(path_in2, item1, name + '.png')
            pngFileOut = os.path.join(path_out, item1, name + '.png')
            print(pngFile1, pngFile2)

            diffValue = diffimg.diff(pngFile1, pngFile2, delete_diff_file=True)
            print(diffValue)

            im1 = Image.open(pngFile1)
            im2 = Image.open(pngFile2)
            difference = ImageChops.difference(im1, im2)

            diffIm1 = ImageOps.fit(im2, difference.size)
            R, G, B, A = difference.split()
            mask = Image.new('L', diffIm1.size, 255)
            diffIm2 = Image.merge('RGBA', (mask, G, B, A)) 
            diffIm2.paste(diffIm1, (0, 0), diffIm1)
            diffIm2.save(pngFileOut)