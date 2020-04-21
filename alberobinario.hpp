#ifndef ALBEROBINARIO_HPP
#define ALBEROBINARIO_HPP

#include <iostream>
#include <iterator>
#include <cstddef>
#include <stdexcept>

/**
    @brief eccezione nodo duplicato
*/
class duplicated_node_exception: public std::runtime_error {
    public:
        duplicated_node_exception(): std::runtime_error("non puoi inserire un nodo uguale ad uno gia' inserito"){}
};

/**
    @brief eccezione nodo non trovato
*/
class node_not_found_exception: public std::runtime_error {
    public:
        node_not_found_exception(): std::runtime_error("nodo non trovato"){}
};

/**
    @brief Albero binario di ricerca

    @param T tipo di dato
    @param D funtore di comparazione < di due dati
    @param E funtore di comparazione == di due dati
**/

template<typename T, typename D, typename E>
class albero_binario{
    
    /**
        Struttura di supporto interna privata che rappresenta i nodi di cui si compone l'albero.

        @brief nodo dell'albero
    */
    struct node{
        
        /**
            @brief valore del nodo di tipo T
        */
        T value; 
        /**
            @brief puntatore al nodo figlio sinistro
        */
        node *l_node; 
        /**
            @brief puntatore al nodo figlio destro
        */
        node *r_node;
        /**
            @brief puntatore al nodo inserito nell'albero direttamente dopo questo
        */ 
        node *next;

        /**
            @brief Costruttore di default
        */
        node() : l_node(nullptr), r_node(nullptr), next(nullptr) {}

        /**
            @brief Costruttore secondario

            @param val valore del nodo
        */
        node(const T &val): value(val), l_node(nullptr), r_node(nullptr), next(nullptr) {}

        /**
            @brief Costruttore secondario che inizializza tutte le sue variabili.

            @param value valore del nodo
            @param next puntatore al nodo inserito nell'albero direttamente dopo questo
            @param l puntatore al nodo figlio sinistro
            @param r puntatore al nodo figlio destro
        */
        node(const T value, node *next, node *l, node *r): value(value), next(next), l_node(l), r_node(r) {}

        /**
            @brief costruttore copia

            @param other il nodo da copiare
        */
        node(const node &other): value(other.value), l_node(other.l_node), r_node(other.r_node), next(other.next) {}
        
        /**
            @brief distruttore del nodo
        */
        ~node(){
            l_node = nullptr;
            r_node = nullptr;
            next = nullptr;
        }
    };  // struct nodo

    /**
            @brief nodo che rappresenta la redice dell'albero
    */
    node *root; 
    /**
            @brief numero di nodi inseriti nell'albero
    */
    unsigned int num_nodi; 
    /**
            @brief oggetto funtore per l'ordinamento
    */
    D different;
    /**
            @brief oggetto funtore per l'eguaglianza
    */ 
    E equal;

    /**
        @brief metodo helper ricorsivo per il subtree

        @param n nodo radice
        @param ret puntatore al sotto-albero da creare

        @throw eccezione generica per l'allocazione della memoria
        @throw eccezione inserimento valore duplicato
    */
    void subtree_helper(const node *n, albero_binario *ret) const{
        if(n == nullptr){
            return;
        } else {
            ret->add(n->value);
            subtree_helper(n->l_node, ret);
            subtree_helper(n->r_node, ret);
        }
    }

    /**
        @brief metodo helper ricorsivo per la stampa in order

        @param n nodo radice
    */
    void in_order_helper(const node* n) const{
        if(root != nullptr){
            if(n->l_node != nullptr) in_order_helper(n->l_node);
            std::cout << n->value << " ";
            if(n->r_node != nullptr) in_order_helper(n->r_node);
        } else{
            std::cout << "L'albero e' vuoto" << std::endl;
        }
    }

    /**
        @brief metodo helper ricorsivo per la stampa "ad albero"

        @param root nodo radice
        @param space spazio tra un livello e l'altro
    */
    void print_h_helper(const node *root, int space) const{
        if(root == nullptr) return;
        space += 10;
        print_h_helper(root->r_node, space);
        std::cout << std::endl;
        for(int i=10; i<space; i++)
            std::cout << " ";
        std::cout << root->value << "\n";
        print_h_helper(root->l_node, space);    
    }

public:

    /**
        @brief costruttore di default
    */ 
    albero_binario(): num_nodi(0), root(nullptr) {} 

    /**
        @brief costruttore copia

        @param other albero da copiare

        @throw eccezione generica per l'allocazione della memoria
        @throw eccezione inserimento valore duplicato

    */ 
    albero_binario(const albero_binario &other): root(nullptr), num_nodi(0) { 
        node *curr = other.root;
        while(curr != nullptr){
            add(curr->value);
            curr = curr->next;
        }
    }

    /**
        @brief distruttore dell'albero
    */ 
    ~albero_binario() { 
        node *current(root);

        while(current){
            node *next(current->next);
            delete current;
            current = next;
        }
    }

    /**
        @brief operatore di assegnamento

        @param other albero da copiare
        @return reference a this

        @throw eccezione generica per l'allocazione della memoria
        @throw eccezione inserimento valore duplicato
    */
    albero_binario &operator=(const albero_binario &other){
        if(this != &other){
            albero_binario tmp(other);
            std::swap(root, tmp.root);
            std::swap(num_nodi, tmp.num_nodi);
        }
        return *this;
    }

    /**
        @brief metodo get del numero di nodi

        @return num_nodi 
        
    */
    const unsigned int get_num_nodi() const {
        return num_nodi;
    }

    /**
        @brief metodo get del nodo root

        @return reference a root
        
    */
    const node& get_root() const {
        return *root;
    }

    /**
        @brief ottiene il nodo richiesto

        @param n valore da cercare
        @return il nodo cercato
        
        @throw node_not_found_exception nel caso in cui il nodo non esista
    */
    node get_node(const T &n) const {
        if(!check_node(n)){
            throw node_not_found_exception();
        }
        node *x = this->root;
        while(x != nullptr && n != x->value){
            if(different(n, x->value))
                x = x->l_node;
            else x = x->r_node;
        }
        return *x;
    }

    /**
        @brief controllo esistenza di un nodo

        @param val valore da cercare
        @return valore booleano contente la risposta
        
    */
    const bool check_node(const T &val) const{
        node *curr = this->root;
        while(curr != nullptr){
                if(equal(curr->value, val))
                    return true;

                if(different(curr->value, val))
                    curr = curr->r_node;
                else curr = curr->l_node;   
        }   
        return false;
    }

    /**
        @brief inserisce nuovo nodo nell'albero

        @param val valore da inserire

        @throw eccezione generica per l'allocazione della memoria
        @throw eccezione inserimento valore duplicato 
    */
    void add(const T &val){
        node *tmp;

        try{
            tmp = new node(val);

            if(root == nullptr){
                root = tmp;
                num_nodi++;
                return;
            }

            node *ultimo = root;
            bool a = true;
            while(a){
                if(ultimo->next != nullptr)
                    ultimo = ultimo->next;
                else a = false;
            }
        
        

            node *curr = root;
            while(curr!=nullptr){
                if(equal(curr->value, val)){
                    throw duplicated_node_exception();
                }

                if(different(curr->value,val)){
                    if(curr->r_node == nullptr){
                        curr->r_node = tmp;
                        ultimo->next = tmp;
                        num_nodi++;
                        return;
                    } else{
                        curr = curr->r_node;
                    }
                }else {
                    if(curr->l_node == nullptr){
                        curr->l_node = tmp;
                        ultimo->next = tmp;
                        num_nodi++;
                        return;
                    } else {
                        curr = curr->l_node;
                    }
                }
            }

        }catch(const std::exception &ex){
            std::cerr << ex.what() << std::endl;
            delete tmp;
        }
    }

    /**
        @brief effettua il subtree come richiesto dalla traccia


        @param d valore da inserire
        @return albero binario di ricerca

        @throw node_not_found_exceptions nel caso in cui il nodo non esista
        @throw eccezione generica per l'allocazione della memoria
        @throw eccezione inserimento valore duplicato
    */
    const albero_binario subtree(const T &d) {
        node r;
        try{    
            r = get_node(d);
        } catch(const node_not_found_exception &e){
            std::cerr << e.what() << std::endl;
            return albero_binario();
        }
        r.next = nullptr;
        albero_binario ret; 
        subtree_helper(&r, &ret);
        return ret;
    }

    /**
        @brief Iteratore costante della lista
    */
    class const_iterator {

        const node *n;

    public:

        typedef std::forward_iterator_tag iterator_category;
        typedef T                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef const T*                  pointer;
        typedef const T&                  reference;


        //costruttore di default
        const_iterator(): n(nullptr){}

        //costruttore copia
        const_iterator(const const_iterator &other): n(other.n){}

        //operatore di asseganmento 
        const_iterator& operator=(const const_iterator &other){
            n = other.n;
            return *this;
        }

        //distruttore
        ~const_iterator(){}

        //ritorna il dato riferito dall'iteratore
        reference operator*() const{
            return n->value;
        }

        //ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const{
            return &(n->value);
        }

        //operatore di iterazione post-incremento
        const_iterator operator++(int){
            const_iterator tmp(*this);
            n = n->next;
            return tmp;
        }

        //operatore di iterazione pre-incremento
        const_iterator& operator++(){
            n = n->next;
            return *this;
        }

        //Operatore di uguagilanza
        bool operator==(const const_iterator &other) const {
            return (n == other.n);
        }

        //operatore di disuguaglianza
        bool operator!=(const const_iterator &other) const {
            return (n != other.n);
        }

    private:

        friend class albero_binario;

        //costruttore privato di inizializzazione usato dalla classe container
        const_iterator(const node *n): n(n) {}

    }; //classe const_iterator

    /**
        @brief iteratore all'inizio della sequenza dati

        @return iteratore all'inizio della sequenza 
    */
    const_iterator begin() const{
        return const_iterator(root);
    }

    /**
        @brief iteratore alla fine della sequenza dati

        @return iteratore alla fine della sequenza 
    */
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    /**
        @brief stampa dell'albero con nella modalita' in order
    */
    void in_order() const{
        in_order_helper(root);
        std::cout << std::endl;
    }

    /**
        @brief stampa dell'albero in orizzontale
    */
    void print_h() const{
        print_h_helper(root, 0);
    }
    


}; //class albero_binario

/**
    @brief Ridefinizione dell'operatore di stream per la stampa dei nodi dell'albero. 

    @param os oggetto stream di output
    @param ab albero binario di ricerca da stampare

    @return reference allo stream di output
*/
template <typename T, typename C, typename E>
std::ostream &operator<<(std::ostream &os, const albero_binario<T,C,E> &ab){

    for(typename albero_binario<T,C,E>::const_iterator i = ab.begin(); i!= ab.end(); ++i){
        std::cout << *i << " ";   
    }
    return os;
}


/**
    @brief Stampa i nodi dell'albero che rispettano il predicato P.

    @param T tipo di dato
    @param D funtore di comparazione < di due dati
    @param E funtore di comparazione == di due dati
    @param P predicato da soddisfare per la stampa
    @param ab albero binario da stampare
    @param pred funtore contenente il predicato

*/
template <typename T, typename C, typename E, typename P>
void printIF(const albero_binario<T,C,E> &ab, P pred) {
    
    for(typename albero_binario<T,C,E>::const_iterator i = ab.begin(); i!= ab.end(); ++i){
        if(pred(*i)){
            std::cout << *i << " ";
        }    
    }
}

#endif //ALBEROBINARIO_HPP