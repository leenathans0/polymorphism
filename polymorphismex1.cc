#include <vector>
#include <iostream>
using namespace std;

/*
Runtime Polymorphism example: Inheriting from a base class and overriding base
class methods. Then calling these overridden methods through the base class 
pointers will invoke the proper sub class method during runtime based on the 
object the pointer references.

C++ Rule of Three: If your class explicitly requires a copy constructor or an assignment
operator or a destructor then it probably requires all three.
*/

class Animal {
public:
    Animal();
    Animal(const Animal& x);               //copy ctor
    Animal& operator=(const Animal& rhs); //assignment operator
    virtual void noise();               //virtual so we can overload in subclass
    int* agep;
    virtual ~Animal();
    //destructor must be virtual and must exist since we are using polymorphism
};

Animal::Animal(){
    cout << "Animal ctor" << endl;
    agep = new int;                   //call new from ctor and delete from dtor
}

Animal::Animal(const Animal& x){   //copy ctor 
    //agep=x.agep;
    //line above happens by default if copy ctor does not exist.
    //Not good as shallow copy causes memory leak  
    //happens by default (not good) shallow copy causes memory leak 
    //AND calling delete twice which is undefined behavior
    //since pointer will be deleted both at the end of
    //foo and ALSO deleted at the end of main.
    cout<<"Animal copy ctor called"<<endl;
    agep =new int;                 //allocate new memory for copy
    *agep=*(x.agep);               //deep copy
}

Animal& Animal::operator=(const Animal& rhs){
    if (this==&rhs) return *this; //check for self assignment (ex. a=a;)
    *agep=*(rhs.agep);            //this pointer is implicit for agep
    return *this;
}

Animal::~Animal(){ 
    cout<<"Animal dtor"<<endl;
    delete agep;                  //must delete in dtor if used new in ctor
}

void Animal::noise(){
    cout<<"grr"<<endl;
}

//==============================================================

class Dog : public Animal{
public:
    Dog ();                               //ctor
    Dog(const Dog& );                     //copy ctor 
    Dog& operator=(const Dog& rhs); 
    ~Dog ();                      //is virtual because base class dtor is virtual
    //dtor is virtual whether we explicitly declare it to be or not 
    //since base class dtor is virtual
    static int count;                     
    //static variables are class level. So only one copy exists for 
    //all instances of Dog class.
    bool shots;
    void noise();
    //noise is virtual because base class noise is virtual
    friend ostream& operator<<(ostream& o, const Dog& x);  
    //insertion operator << is declared as a friend of the Dog class.
    //Friendship is given not taken.
};

int Dog::count=0;  //must initialize static members at file scope

Dog::Dog():shots(false){  //Dog ctor 
    //note initialization list syntax :var(default value),
    count++;
    //shots=false; //same as using initialization list above
    //If shots was const, must use initialization list
    cout<<"Dog ctor: There are now "<<count<< " dogs"<<endl;
}

Dog::~Dog(){ //dtor is virtual so Animal dtor will also be called
    count--;
    cout<<"Dog dtor: There are now "<<count<< " dogs"<<endl;
}

Dog::Dog(const Dog& d):Animal(d){ // Dog copy ctor calls base class ctor first
    shots=d.shots; //simple copy not a pointer
    count++;
    cout<<"Copy Ctor of dog: There are now "<<count<< " dogs"<<endl;
}

Dog& Dog::operator=(const Dog& rhs){  //override assignment operator
    //if (this==&rhs) return *this;     //check for self assignment (ex. d=d;)
    cout << "Dog assignment" << endl;
    Animal::operator=(rhs); 
    // call Animal assignment operator function sending rhs by reference
    shots=rhs.shots;
    return *this;  //so multiple assignment will work eg a=b=c
}

void Dog::noise(){
    cout<<"ruf ruf"<<endl;
}

//==============================================================
    
void foo(Dog f){     //copy ctor called
    *(f.agep)=10;    //changing f should NOT change d in main scope
    cout << "printing dog f IN foo "<< f <<endl;
}

ostream& operator<<(ostream& o, const Dog& x){   //cout<<A<<B;
    o << "Age: " << *(x.agep) << " -- " << "Shots: " << x.shots;
    return o;
}

int main(){
    Dog d;       //stack
    *(d.agep)=5;
    d.shots=true;
    foo(d);    //copy ctor called passing d to f in foo
    cout <<"printing dog AFTER foo "<< d  <<endl;
    cout << *(d.agep) << " should still be 5 after foo if copy ctor works"<<endl;
    if (1)
    {
        Dog R;         // Dog R only exists in this, if block scope
        R=d;           //needs overloaded operator=
        cout<<R<<endl; //overloaded friend insertion operator 
    }
    Animal* ap=&d;
    ap->noise();     
    //polymorphism: 
    //Base class Animal pointer ap points to Dog object, so Dog::noise() is called
    //we can now make a vector <Animal*> v and push_back dog, cat, bird pointers.
    //if we iterate through the vector and call ->noise() we will get the correct
    //behavior ruf ruf, meow, tweet for all pointers. This is dynamic binding.

    return 0;
}
