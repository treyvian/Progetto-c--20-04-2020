#include <iostream>
#include <cassert>
#include <string>


#include "alberobinario.hpp"

//funtore di uguaglianza
struct int_equal{
    bool operator()(const int val1,const int val2) const{
        return val1 == val2;
    }    
};

//funtore di disuguaglianza
struct int_different{
    bool operator()(const int val1,const int val2) const{
        return val1 < val2;
    }  
};

//funtore da utilizzare come predicato nel printIF
struct pari{
    bool operator()(const int num) const{
        return num%2==0;
    }
};


typedef albero_binario<int,  int_different, int_equal> aint;

void albero_test_fondamentali(){
    
    std::cout << "***albero_test_fondamentali***" << std::endl; 

    // test add metodo add

    aint a;
    

    a.add(4);

    a.add(4); ///< test prova duplicated node

    a.add(3);
    a.add(6);
    a.add(5);
    

    std::cout << "stampa albero dopo inserimento" << std::endl << a << std::endl;
    a.in_order();

    // test metodo get_root
    assert(a.get_root().value == 4);

    // test costruttore copia
    aint b(a);

    std::cout << "stampa dell'albero copiato" << std::endl << b << std::endl;

    assert(a.get_root().value == b.get_root().value);

    //test operatore di assegnamento
    aint c;
    c = a;

    std::cout << "stampa dell'albero assegnato" << std::endl << c << std::endl << std::endl; 

    assert(a.get_root().value == c.get_root().value);

    //test metodo get_num_nodi
    assert(a.get_num_nodi() == 4);
    assert(b.get_num_nodi() == 4);
    assert(c.get_num_nodi() == 4);

    //test metodo check_node
    bool ok1 = a.check_node(5);
    bool ok2 = a.check_node(3);
    assert(ok1);
    assert(ok2);

    bool ok3 = a.check_node(10);
    assert(!ok3);
}

void albero_test_subtree(){

    std::cout << "***albero_test_subtree***" << std::endl; 

    aint a;

    a.add(6);
    a.add(4);
    a.add(8);
    a.add(2);
    a.add(5);
    a.add(7);
    a.add(9);
    a.add(1);
    a.add(3);

    std::cout << "stampa albero per sub tree" << std::endl << a << std::endl;

    std::cout << "stampa albero print_h" << std::endl;
    //test metodo print_h
    a.print_h();
    
    aint b = a.subtree(8); 

    std::cout << "stampa albero subtree 8" << std::endl << b << std::endl;
    
    std::cout << "stampa albero subtree print_h" << std::endl;
    //test metodo print_h per subtree
    b.print_h();

    aint c = a.subtree(4);

    std::cout << "stampa albero subtree 4" << std::endl << c << std::endl << std::endl;

    assert(c.get_root().r_node->value == 5);

    aint d = a.subtree(12);
    assert(&(d.get_root()) == nullptr);
}

void albero_test_printIF(){

    std::cout << "***albero_test_printIF***" << std::endl; 

    aint a;

    a.add(6);
    a.add(4);
    a.add(8);
    a.add(2);
    a.add(5);
    a.add(7);
    a.add(9);
    a.add(1);
    a.add(3);

    printIF(a, pari());
}

void test_const_list(const aint &other){
    std::cout << "***Test sull'albero costante***" << std::endl;

    std::cout << "***Stampa con operatore<<***" << std::endl;
    std::cout << other << std::endl;

    std::cout << "***Stampa con print_h***" << std::endl;
    other.print_h();

    std::cout << "Stampa dimensione albero: " << other.get_num_nodi() << std::endl;
}

/**
    funtore per il confronto tra stinghe, la valutazione viene svolta sulla lunghezza delle 
    stesse. Ritorna true se la prima stringa e' piu' corta della seconda.

*/
struct compare_string{
    bool operator()(const std::string &a, const std::string &b) const{
        return (a.size()<b.size());
    }
};

/**
    funtore per l'uguaglianza tra stinghe.

*/
struct equal_string{
    bool operator()(const std::string &a, const std::string &b) const{
        return (a == b);
    }
};

typedef albero_binario<std::string,  compare_string, equal_string> astr;

//stessi test effettuati prima ma con la variante delle stringhe
void albero_test_stringhe(){
    std::cout << std::endl << "***Test sull'albero di stringhe***" << std::endl;

    astr a;

    a.add("pianta");
    a.add("casa");
    a.add("astuccio");
    a.add("io");
    a.add("carta");
    a.add("monitor");
    a.add("salvietta");
    a.add("e");
    a.add("tre");
    
    std::cout << "~stampa albero stringa~" << std::endl;
    std::cout << a << std::endl << std::endl;

    std::cout << "~stampa albero stringa print_h~" << std::endl;
    a.print_h();

    std::cout << "~stampa albero stringa in order~" << std::endl;
    a.in_order();

    std::cout << "dimensione della lista: " << a.get_num_nodi() << std::endl;

    bool r1 = a.check_node("tre");
    bool r2 = a.check_node("pane");

    astr b = a.subtree("astuccio");

    std::cout << "~stampa albero stringa subtree~" << std::endl;
    std::cout << b << std::endl;

}

struct point{
    int x;
    int y;

    point(int xx, int yy): x(xx), y(yy) {}
};

struct compare_point{
    bool operator()(const point &p1, const point &p2) const{
        return (p1.x < p2.x);
    }
};

struct equal_point{
    bool operator()(const point &p1, const point &p2) const{
        return (p1.x == p2.x) && (p1.y == p2.y);
    }
};

std::ostream &operator<<(std::ostream &os, const point &p){
    std::cout<<"("<<p.x<<","<<p.y<<")";
    return os;
};


void test_list_int(){
    std::cout << "***Test sull'albero' di punti***" << std::endl;

    albero_binario<point,  compare_point, equal_point> ap;

    ap.add(point(1,1));
    ap.add(point(1,2));
    ap.add(point(2,7));
    ap.add(point(0,0));
    ap.add(point(5,4));

    std::cout << "Stampa con operator<<" << std::endl;
    std::cout << ap << std::endl;

    std::cout << "stampa con print_h" << std::endl;
    ap.print_h();

    std::cout << "Ricerca di '(0,0)': "<< ap.check_node(point(0,0)) << std::endl;
    std::cout << "Ricerca di '(2,2)': "<< ap.check_node(point(2,2)) << std::endl;
}

int main(){
    albero_test_fondamentali();
    albero_test_subtree();
    albero_test_printIF();
    aint other;
    test_const_list(other);
    albero_test_stringhe();
    test_list_int();

    return 0;
}