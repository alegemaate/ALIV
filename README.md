# aliv (Allegro Image Viewer)
A simple Allegro 4 based image viewer.

### Features
Support for opening
- png
- jpeg
- bmp
- gif (spotty)
- pcx
- tga
- tgx (Firefly image format)
- gm1 (Firefly animation format)


### Linking and Compiling
You must link with allegro 4, lib load png (comes with allegro 4) and lib jpeg to compile. These can be found in the following locations:
- [Allegro 4](http://liballeg.org/api.html)
- [LibJpeg](https://github.com/LuaDist/libjpeg)

They should be linked in this order
```
-lloadpng
-lalleg44
-ljpeg
```
