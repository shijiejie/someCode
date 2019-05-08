import numpy as np
import caffe
import collections
from collections import OrderedDict
caffe.set_mode_cpu
net0 = caffe.Net('deploy1.prototxt','trt.caffemodel_dst.caffemodel',caffe.TEST) 
conv1_w = net0.params['conv1/3x3_s1'][0].data
keys0 = net0.params.keys()
print(net0.params.keys())
for key1 in keys0:
    print(keys0)
    try:
        print(net0.params[key1][0].data)
    except IndexError:
        continue
    try:
        print(net0.params[key1][1].data)
    except IndexError:
        continue
    try:
        print(net0.params[key1][2].data)
    except IndexError:
        continue
    finally:
        print("\n")
