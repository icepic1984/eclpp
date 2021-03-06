/* -*- mode: c;  -*-
   file: app_main.c
*/

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "app_main.h"

#define DEFUN(name, fun, args)                                                 \
    cl_def_c_function(c_string_to_object(name), (cl_objectfn_fixed)fun, args)

void run_swank();

cl_object set_runtime(cl_object i)
{
    int seconds = fix(i);
    return ecl_make_integer(seconds);
}

struct Image{
    int width;
    int height;
};

static Image image;
// https://gitlab.com/embeddable-common-lisp/ecl/issues/324
cl_object make_image()
{
    return ecl_make_foreign_data(ECL_NIL, sizeof(Image),&image);
}

cl_object set_width(cl_object image, cl_object width)
{
    Image* i = static_cast<Image*>(ecl_foreign_data_pointer_safe(image));
    i->width = fix(width);
    return image;
}

cl_object get_width(cl_object image)
{
    Image* i = static_cast<Image*>(ecl_foreign_data_pointer_safe(image));
    return ecl_make_fixnum(i->width);
}

/* TODO: Are embedded quotes really needed? */
/* char start_swank[] = */
/*   "\"/mnt/pixel-512/dev/stupid-ecl-tricks-1/start-swank-server.lisp\""; */
//char start_swank[] = "\"/home/icepic/quicklisp/dists/quicklisp/software/slime-v2.22/start-swank.lisp\"";

char start_swank[] = "\"/home/icepic/Code/ecl/swank/start-swank.lisp\"";

char* argv;
char** pargv;

int app_main() {
  argv = "app";
  pargv = &argv;

  cl_boot(1, pargv);
  atexit(cl_shutdown);

  /* Set up handler for Lisp errors to prevent buggy Lisp (an */
  /* imposibility, I know!) from killing the app. */
  const cl_env_ptr l_env = ecl_process_env();
  CL_CATCH_ALL_BEGIN(l_env) {
    CL_UNWIND_PROTECT_BEGIN(l_env) {
      run_swank();
    }
    CL_UNWIND_PROTECT_EXIT {}
    CL_UNWIND_PROTECT_END;
  }
  CL_CATCH_ALL_END;

  while(1){};
  return 0;

}

void run_swank() {
  cl_object cl_start_swank_path = c_string_to_object(start_swank);
  cl_object cl_load =  ecl_make_symbol("LOAD","CL");
  cl_funcall(2, cl_load, cl_start_swank_path);
  DEFUN("set-runtime", set_runtime, 1);
  DEFUN("make-image", make_image,0);
  DEFUN("set-width", set_width,2);
  DEFUN("get-width", get_width,1);
  return;
}
