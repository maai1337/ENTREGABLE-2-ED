#include <iostream>
#include "procesar_libros.hpp"
#include "tree.hpp"

int main() {
    std::cout << "Construyendo el arbol basado en los archivos XML\n";
    Tree tree = ProcesarLibros::construirArbol();
    std::cout << "Arbol construido. Nodos totales: " << tree.size() << "\n";

	std::cout << "Lista de libros del arbol: \n";
    tree.listar();

	std::cout << "\nLista de libros precursores: \n";
    tree.precursores();

    int tamano_antes = tree.size();
    std::cout << "\nNodos antes de borrar: " << tamano_antes << "\n";

    tree.borrar_ratings(4);

    int tamano_despues = tree.size();
    std::cout << "Nodos despues de borrar: " << tamano_despues << "\n";
    std::cout << "Nodos eliminados en total: " << (tamano_antes - tamano_despues) << "\n";

    std::cout << "\nIDs restantes despues de usar borrar_ratings: \n";
    tree.listar();
    return 0;
}