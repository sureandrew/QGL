%module(directors="1") director_frob;
#pragma SWIG nowarn=SWIGWARN_TYPEMAP_THREAD_UNSAFE,SWIGWARN_TYPEMAP_DIRECTOROUT_PTR

%header %{
#include <iostream>
%}

%feature("director");
%feature("nodirector") Bravo::abs_method();   // ok
%feature("director")   Charlie::abs_method(); // ok
%feature("nodirector") Delta::abs_method();   // ok

%inline %{

  struct Alpha
  {
    virtual ~Alpha() { };
    virtual const char* abs_method() = 0;
  };
  
  struct Bravo : Alpha
  {
    const char* abs_method()
    {
      return "Bravo::abs_method()";
    }
  };

  struct Charlie : Bravo 
  {
    const char* abs_method()
    {
      return "Charlie::abs_method()";
    }
  };
  
  struct Delta : Charlie
  {
  };
%}

%rename(OpInt) operator int();
%rename(OpIntStarStarConst) operator int **() const;
%rename(OpIntAmp) operator int &();
%rename(OpIntStar) operator void *();
%rename(OpConstIntIntStar) operator const int *();

%inline %{
  class Ops {
  public:
    Ops() : num(0) {}
    virtual ~Ops() {}
    virtual operator int() { return 0; }
    virtual operator int **() const {
      return (int **) 0;
    }
    virtual operator int &() {
      return num;
    }
    virtual operator void *() {
      return (void *) this;
    }
    virtual operator const int *() {
      return &num;
    }
  private:
    int num;
  };

  struct Prims {
    virtual ~Prims() {}
    virtual unsigned long long ull(unsigned long long i, unsigned long long j) { return i + j; }
    unsigned long long callull(int i, int j) { return ull(i, j); }
  };
%}
