#ifndef __GECODESPACE_H
#define __GECODESPACE_H


#ifndef MOZART_GENERATOR

//#include "gecode-space-decl.hh"

namespace mozart {
  GecodeSpace::GecodeSpace(void)
    : Gecode::Space(){
    std::cout << "Constructed gecode space" << std::endl;
  }
  
GecodeSpace::GecodeSpace(bool share, GecodeSpace& other) 
  : Gecode::Space(share,other),
    _intVars(other._intVars),
    _setVars(other._setVars)
{
  for(auto i = _intVars.size(); i--;)
    _intVars[i].update(*this,share,other._intVars[i]);
  for(auto i = _setVars.size(); i--;)
    _setVars[i].update(*this,share,other._setVars[i]);
}

GecodeSpace::~GecodeSpace(void) {
  std::cout << "Destructed gecode space" << std::endl;
}
  
int GecodeSpace::propagate(void){
  //std::cout << "GecodeSpace: propagating... " <<  std::endl;
  return (int)(this->status());
}
//This methods updates all the vars of *this* gecodeSpace with
//the information available in *other* gecodeSpace.
void GecodeSpace::reflectVars(GecodeSpace& other){
  for(unsigned int i = 0; i<_intVars.size(); ++i)
    _intVars[i].update(*this,false,other._intVars[i]);
  /*for(auto i = _setVars.size(); i--;)
    _setVars[i].update(*this,share,other._setVars[i]);*/
}
  
Gecode::Space* GecodeSpace::copy(bool share) {
  return new GecodeSpace(share,*this);
}
  
Gecode::IntVar& GecodeSpace::intVar(size_t index) {
  if (index >= _intVars.size())
    std::cerr << "Accessing intVar at invalid index" << std::endl;
  return _intVars[index];
}
  
size_t GecodeSpace::newIntVar(int min, int max) {
  _intVars.push_back(Gecode::IntVar(*this,min,max));
  return _intVars.size() - 1;
}
  
Gecode::SetVar& GecodeSpace::setVar(size_t index) {
  if (index >= _setVars.size())
    std::cerr << "Accessing setVar at invalid index" << std::endl;
  return _setVars[index];
}
size_t GecodeSpace::newSetVar() {
  _setVars.push_back(Gecode::SetVar());
  return _setVars.size() - 1;
}
Gecode::BoolVar& GecodeSpace::boolVar(size_t index) {
  if (index >= _boolVars.size())
    std::cerr << "Accessing setVar at invalid index" << std::endl;
  return _boolVars[index];
}
size_t GecodeSpace::newBoolVar() {
  _boolVars.push_back(Gecode::BoolVar());
  return _boolVars.size() - 1;
}
void GecodeSpace::dumpSpaceInformation(void) const {
  std::cout << "Space information" 
	    << std::endl
	    << "\tInteger variables: " << _intVars.size() << std::endl
	    << "\tset variables: " << _setVars.size() << std::endl;
  for(unsigned int i=0; i<_intVars.size(); ++i){
    if(_intVars[i].size()==1)
      std::cout << "FD variable in pos " << i+1 << " is assigned to value " << _intVars[i].val() << std::endl;
  }
}
}

#endif

#endif
