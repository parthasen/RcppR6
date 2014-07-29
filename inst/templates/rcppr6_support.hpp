// Generated by rcppr6 ({{version}}): do not edit by hand
#ifndef _{{PACKAGE}}_RCPPR6_SUPPORT_HPP_
#define _{{PACKAGE}}_RCPPR6_SUPPORT_HPP_

// These functions will be needed by all packages that use the
// generation approach, so we'll want to install this.

// Ideally this would actually become fully fledged R6 support for
// Rcpp though, with access to various members, etc.  Not sure if
// that's going to be any faster though, but it might be better.

// Eventually, it would be nice to be able to do the Rcpp type traits
// thing where we can do:
//
// Rcpp::as<R6<T> >(SEXP)
//
// And then provide nice support for passing pointer dereferencing etc
// around.  This will actually be very easy and I'll get to work on
// that shortly I think.
//
// At present, everything in here should be considered an
// implementation detail and not API.  Everything will change.
// Consequences will never be the same.

// This is going to need to be loaded during rcpp_post.hpp.
#include <Rcpp.h>

namespace {{package}} {
namespace rcppr6 {

// Get the top level class attribute from an object.  Returns the
// empty string if it's not there, or if it's the zero length
// character vector.  Could be modified to return the i'th class
// element easily enough, but at that point we might as well return
// the whole bloody thing.
inline std::string get_class(Rcpp::RObject x) {
  if (x.hasAttribute("class")) {
    Rcpp::CharacterVector cx(x.attr("class"));
    if (cx.size() > 0) {
      return Rcpp::as<std::string>(cx[0]); // Always the first element.
    } else {
      return "";
    }
  } else {
    return "";
  }
}

template <typename T>
void check_ptr_valid(Rcpp::XPtr<T> p) {
  T* test = p;
  if (test == NULL) {
    Rcpp::stop("Pointer is NULL");
  }
}

// Would be nice to store a static reference to somewhere down this
// chain on load.  But that's premature optimisation.  Let's find out
// if this is slow.
//
// It might be better to provide first-class access to R6 bits,
// actually.  That could possibly be done through a persistant hash
// (std::map even) so that the first lookup is costly and others are
// free.
//
// So much of this probably should be cached and not recomputed each
// time.  Or done during template instantiation or something.  I think
// that we can get at least to getNamespace on package load, via a
// static variable and a hook, but probably the entire way.  I'd be OK
// with a load hook if that really helps.  We could also do it via a
// static variable that is initially set to R_NilValue first, I
// think.
//
// TODO: This should not be ptr_to_R6 but obj_to_R6 or obj_to_ptr_to_R6
template <typename T>
SEXP ptr_to_R6(T x, std::string classname) {
  Rcpp::Environment base("package:base");
  Rcpp::Function getNamespace = base["getNamespace"];
  Rcpp::Environment pkg = getNamespace("{{package}}");
  Rcpp::List Generator = pkg[".R6_" + classname];
  Rcpp::Function Generator_new = Generator["new"];
  // TODO: Not sure about embedding package/classname this way.
  Rcpp::XPtr<T> ptr(new T(x), true);
  ptr.attr("type") = "{{package}}__" + classname;

  return Generator_new(ptr);
}

// Getting classname here as a template specificiation would be a nice
// addition, then we'd use this as
//    ptr_from_R6<"big_int">(x);
template <typename T>
Rcpp::XPtr<T> ptr_from_R6(Rcpp::RObject x,
                          std::string classname) {
  if (get_class(x) == classname) {
    Rcpp::Environment xe = Rcpp::as<Rcpp::Environment>(x);
    Rcpp::XPtr<T> ptr = Rcpp::as<Rcpp::XPtr<T> >(xe.find("ptr"));
    check_ptr_valid<T>(ptr);
    return ptr;
  } else {
    Rcpp::stop("Expected an object of type R6 / " + classname);
    // This piece of code will never run (ideally Rcpp::stop would be
    // marked noreturn [which I think it actually is on gcc but not
    // for clang]).  But to avoid a compiler warning we need to appear
    // to return something here.
    return Rcpp::as<Rcpp::XPtr<T> >(x);
  }
}

}
}

#endif