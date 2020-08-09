#ifndef AbstractInterface_H_
#define AbstractInterface_H_


class AbstractInterface {

  public:
  virtual void begin() {};

  virtual void check() {};
  virtual void work()  {};
  virtual void idle()  {};
  virtual void clean() {};

};



#endif