#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "tinyxml2.h"
#include "tree.hpp"
#include "procesar_libros.hpp"

using namespace tinyxml2;

// Crea un nodo con una etiqueta y otro nodo hijo con el valor.
// Estructura: parent -> tag_node -> value_node
// Esta función es auxiliar para crear campos de tipo clave-valor en el árbol.
void ProcesarLibros::addCampo(Tree& tree, Tree::Node* parent, const std::string& tag, const std::string& value) {
    Tree::Node* tagNode = tree.insertChild(parent, tag);
    tree.insertChild(tagNode, value);
}

// Retorna el texto de un elemento XML de forma segura.
// Si el elemento es nullptr o no contiene texto, devuelve un string vacío en lugar de lanzar una excepción.
// Esta función es crucial para manejar campos vacíos en los archivos XML sin causar problemas.
// Parámetro: element - Elemento XML del cual extraer el texto
// Retorna: El texto del elemento o un string vacío si no hay contenido
std::string ProcesarLibros::safeText(XMLElement* element) { 
    const char* text = element ? element->GetText() : nullptr;
    return text ? text : "";
}

// Procesa la lista de libros similares y los inserta como subnodos del libro actual.
// Busca dentro del elemento XML "similar_books", itera sobre cada libro similar encontrado,
// extrae sus datos principales (título, ISBN, año de publicación) y los inserta en el árbol
// bajo un nodo llamado "similar_books" dentro del libro principal.
// Parámetros:
//   - similarBooks: Elemento XML que contiene la lista de libros similares
//   - tree: Referencia al árbol donde se insertan los datos
//   - parent: Nodo padre donde se crea el nodo "similar_books"
void ProcesarLibros::procesarSimilarBooks(XMLElement* similarBooks, Tree& tree, Tree::Node* parent) {
    Tree::Node* similarNode = tree.insertChild(parent, "similar_books");

    // Itera sobre cada libro similar y extrae sus datos principales (título, ISBN, año)
    XMLElement* book = similarBooks->FirstChildElement("book");
    while (book) {
        Tree::Node* bookNode = tree.insertChild(similarNode, "book");
        addCampo(tree, bookNode, "title", safeText(book->FirstChildElement("title")));
        addCampo(tree, bookNode, "isbn", safeText(book->FirstChildElement("isbn")));
        addCampo(tree, bookNode, "publication_year", safeText(book->FirstChildElement("publication_year")));
        book = book->NextSiblingElement("book");
    }
}

// Procesa un libro individual: extrae todos sus datos del XML y los inserta como nodos en el árbol.
// Para cada libro, se crea un nodo "book" y se agregan como subnodos: id, título, ISBN,
// año de publicación, código de idioma, descripción, calificación promedio y número de páginas.
// Además, procesa y agrega los libros similares asociados al libro actual.
// Parámetros:
//   - book: Elemento XML del libro a procesar
//   - tree: Referencia al árbol donde se insertan los datos
//   - parent: Nodo padre
//   - index: Índice del libro en la secuencia de procesamiento
void ProcesarLibros::procesarLibro(XMLElement* book, Tree& tree, Tree::Node* parent, int index) {

    Tree::Node* bookNode = tree.insertChild(parent, "book");

    // Agrega los campos principales del libro usando la función auxiliar addCampo
    addCampo(tree, bookNode, "id", safeText(book->FirstChildElement("id")));
    addCampo(tree, bookNode, "title", safeText(book->FirstChildElement("title")));
    addCampo(tree, bookNode, "isbn", safeText(book->FirstChildElement("isbn")));
    addCampo(tree, bookNode, "publication_year", safeText(book->FirstChildElement("publication_year")));
    addCampo(tree, bookNode, "language_code", safeText(book->FirstChildElement("language_code")));
    addCampo(tree, bookNode, "description", safeText(book->FirstChildElement("description")));
    addCampo(tree, bookNode, "average_rating", safeText(book->FirstChildElement("average_rating")));
    addCampo(tree, bookNode, "num_pages", safeText(book->FirstChildElement("num_pages")));

    // Procesa y agrega los libros similares como subnodos
    procesarSimilarBooks(book->FirstChildElement("similar_books"), tree, bookNode);
}

// Construye el árbol completo leyendo y procesando todos los archivos XML del directorio books_xml.
// Esta es la función principal que:
//   1. Crea un árbol vacío con capacidad para 10000 nodos
//   2. Establece la estructura raíz: "GoodreadsResponse" -> "books"
//   3. Itera sobre cada archivo XML en el directorio books_xml
//   4. Carga cada archivo XML y extrae los datos del libro
//   5. Procesa cada libro de forma individual
// Retorna: Un objeto Tree completamente construido con todos los libros y sus datos.
Tree ProcesarLibros::construirArbol() {
    const std::string dir = "books_xml";
    Tree tree(10000);  // Crea un árbol con máximo 10000 nodos de capacidad
    Tree::Node* rootNode = tree.insertRoot("GoodreadsResponse");  // Nodo raíz
    Tree::Node* booksNode = tree.insertChild(rootNode, "books");  // Contenedor de todos los libros

    ProcesarLibros procesador;

    // Itera sobre cada archivo XML encontrado en el directorio books_xml
    int index = 1;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        XMLDocument doc;
        XMLError eResult = doc.LoadFile(entry.path().string().c_str());
        XMLElement* root = doc.FirstChildElement("GoodreadsResponse");
        XMLElement* book = root->FirstChildElement("book");
        procesador.procesarLibro(book, tree, booksNode, index);
        index++;
    }

    return tree;
}

