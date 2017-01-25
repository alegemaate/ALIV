# aliv
A simple Allegro 4 based image viewer.
#### Linking
You must link with allegro 4, allegro png, jpeg alleg and allegro gif to compile. These can be found in the following locations:
- [Allegro 4](http://liballeg.org/api.html)
- [alpng](http://alpng.sourceforge.net/)
- [algif](http://algif.sourceforge.net/)
- [jpgalleg](http://devpaks.org/details.php?devpak=70)

They should be linked in this order
```
-lalpng
-ljpgal
-lalgif
-lalleg
```
