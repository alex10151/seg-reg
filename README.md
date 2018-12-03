# configurations on linux
## step1 swig 
make sure you have `Pipeline.i`:
```sh
$swig -c++ -python Pipeline.i
```
## step2 cmake
you now got two files :`SegAndRegPipe.py` and `Pipeline_wrap.cxx`.to generate a `.so` file you should run:
```sh
$cmake .
$make
```
then rename the `.so` file to `_SegAndRegPipe.so`
## step3 invoke
just `import` in your python script:
```python
>>>>>import SegAndRegPipe as sg
```
# using with flask 
run:
```sh
$ python app.py 
```
to start a local server.

then you can send your requests.there are two interfaces so far:

url = /seg method = POST

for segmentation(bones,table removal)

url= /reg method = POST

for regitration (PET-CET)

both interfaces need to pass `raw image data`.

# 3DimageSegmentation
## Module 1 region grow with confidence
* see `RegionGrowMethod.h` for details
## Module 2 table removal (based on line detection)
* see `TableRemoval.h` for details
## Module 3 bone removal 
### head bone
* threshold process ->LoG caculator(edge detection)->fusion-> close operation
### body bone
* threshold process ->close operation->hole fill filter
# CT-PET Registration
## method 1 multiresolution
* see `RegistrationPipe.h` for details
