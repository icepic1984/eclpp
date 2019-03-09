;; Build: 
;; Compile lib g++ `ecl-config --cflags` lib.cpp -shared -fPIC -o lib.so `ecl-config --libs`
;; ecl -compile test.lisp
;; ecl -load test.fas

(ffi:load-foreign-library "/home/icepic/Code/ecl/swank/lib.so")
(ffi:def-function ("init_lib" init) NIL :returning :void)

;(init-extlib)


;(load "/home/icepic/Code/ecl/swank/test.fas")
