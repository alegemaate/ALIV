#include "image_data.h"

// Constructor
image_data::image_data(BITMAP *image, std::string title, std::string errorMessage) {
  // Image
  this -> image = image;

  // Width and height
  if (image != NULL) {
    width = this -> image -> w;
    height = this -> image -> h;
  }

  // Ratio
  wh_ratio = float(width) / float(height);
  hw_ratio = float(height) / float(width);

  // Wide or high
  if( wh_ratio > 1.0f)
    wide = true;
  else
    wide = false;

  // Image path
  filePath = title;

  // Error message
  this -> errorMessage = "Error: " + errorMessage;

  // Empty error?
  if (errorMessage == "") {
    errorMessage = "Error: Could not load image" + filePath;
  }
}
