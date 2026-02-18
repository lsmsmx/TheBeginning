/*#include <iostream>
int main() {
    std::cout << "HAII:3" << std::endl;
    return 0;
}*/
/*
if ((0<i<16)&&(i%1==0)) {
      bit_i = A & (1<<i);
   }
   else {
      std::cout << "Повторно введите индекс, убедитесь, что выполняется условия: 0<i<16, а также i - целое "<< std::endl;
      std::cin >> i;*/
/*#include <iostream>
main(){
   /* int x = 10
    x & (1<<3);
    */
/*  int i = 1;
   while (true){
      if (i > 10)
         break;
      std::cout << i << " ";
      i+=2;
   }
   std::cout << "end" << std::endl;
   for (int a = 1; i <= 10; ++i) {
      if (i %2 != 0) {
         continue;
      }
      std::cout << i <<" ";
   }

   int i = 0;
   do {
      std:
   }*/
/*int x = 113;
for int d = 2; d < x; ++d) {
   if (x%d==0) {
      std::cout << "its not prime" << std::endl;
      break
   }
}
bool isPrime = true;
int x = 4;
for (int d = 2; d < x; ++d) {
   if (x%d==0) {
      isPrime = false;
      break;
   }
   if (isPrime) {
      std::cout << "its not prime" << std::endl;
   }
   } */
  
  /* char x;
  std::cin >> x;
  scanf("")
  short s;
  std::cin >> s;
  char 
} 
*/
#include <iostream>
/*void move (int& x, int& y) {
   x += 10;
   y += 15;
}

bool intersect(Car c1, Car c2) {

}*/

// 1 принцип Абстракция
// 2 принцип Инкапсулляция
// x, y
// struct - хранит
// class - хранит, описывает поведения

class Car {
private:
   bool is_r_valid(float r) {
      
   }
private:
   // поле
   int x_ = 10;
   int y_ = 10;
   float r_ = 1.5f;
   int* arr_;
   int n_;

   // метод
public:
   Car(int n) : arr_(new int[n_]),  n_(n) {
      std::cerr << "ctor" << std::endl;
   }
   ~Car() {
      std::cerr << "dtor" << std::endl;
   }
   Car(int x, int y, float r) : x_(x), y_(y), r_(r) {
      std::cerr << "Ctor param" << std::endl;
   }
   void move() {
      x_ += 10;
      y_ += 10;
   }
   void set_r(float r) {
      if (r < 0.01) {
         throw std::invalid_argument("Radius has to be invalid");
      }
      r_ = r;
   }
   int get_x() const {return x_;}
};

void intersect(const Car& c1, const Car& c2) {
   c1.get_x(); 
}

int main()

{
   try {
      Car car(100); // объект класса
      car.set_r(-10.f);   
      car.set_r(1);
   }
   catch(const std::exception& ex) {
      std::cerr << ex.what() << std::endl;
   }

   }