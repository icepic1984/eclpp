#include <ecl/ecl.h>
#include <iostream>

struct Image {
    Image()
    {
        std::cout << "Constructor" << std::endl;
    }
    ~Image()
    {
        std::cout << "Deconstructor" << std::endl;
    }

    int width;
    int height;
};

cl_object poof_my_class(cl_object h)
{
    Image* foo = static_cast<Image*>(ecl_foreign_data_pointer_safe(h));
    printf("Poofing foo!\n");
    delete foo;
    return ECL_T;
}

// https://gitlab.com/embeddable-common-lisp/ecl/issues/324
cl_object make_image()
{

    std::cout << "Make image" << std::endl;
    auto foreign = ecl_make_foreign_data(ECL_NIL, sizeof(Image), new Image());

    auto finalizer =
        ecl_make_cfun(reinterpret_cast<cl_objectfn_fixed>(poof_my_class),
            ecl_read_from_cstring("finalizer"), ECL_NIL, 1);
    si_set_finalizer(foreign, finalizer);

    // auto g  = si_get_finalizer(foreign);
    auto t = ecl_t_of(finalizer);
    if (t == t_cfun)
        std::cout << "yeah" << std::endl;
    if (t == t_cfunfixed)
        std::cout << "yeah2" << std::endl;
    std::cout << t << std::endl;

    return foreign;
}

cl_object set_width(cl_object image, cl_object width)
{
    Image* i = static_cast<Image*>(ecl_foreign_data_pointer_safe(image));
    i->width = fix(width);
    return ECL_NIL;
}

cl_object get_width(cl_object image)
{
    Image* i = static_cast<Image*>(ecl_foreign_data_pointer_safe(image));
    return ecl_make_fixnum(i->width);
}

extern "C" {
void init_lib(void)
{
    std::cout << "Yeah" << std::endl;
    // https://stackoverflow.com/questions/44494613/embeddable-common-lisp-ecl-load-shared-library-with-extension-functionsxo
    ecl_def_c_function(ecl_read_from_cstring("make-image"),
        reinterpret_cast<cl_objectfn_fixed>(make_image), 0);
    ecl_def_c_function(ecl_read_from_cstring("set-width"),
        reinterpret_cast<cl_objectfn_fixed>(set_width), 2);
    ecl_def_c_function(ecl_read_from_cstring("get-width"),
        reinterpret_cast<cl_objectfn_fixed>(get_width), 1);
}
}
