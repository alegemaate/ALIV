# aliv (Allegro Image Viewer)
A simple Allegro 4 based image viewer.
#### Linking
You must link with allegro 4, allegro png, allegro gif and lib jpeg to compile. These can be found in the following locations:
- [Allegro 4](http://liballeg.org/api.html)
- [alpng](http://alpng.sourceforge.net/)
- [algif](http://algif.sourceforge.net/)
- [LibJpeg](https://github.com/LuaDist/libjpeg)

They should be linked in this order
```
-lalpng
-ljpeg
-lalgif
-lalleg
```
