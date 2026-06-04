#include <string>
#include "tinyxml2.h"
#include "tree.hpp"

// Clase encargada de procesar archivos XML de libros y construir un árbol jerárquico.
// Lee archivos XML individuales de libros desde la carpeta books_xml, extrae sus datos
// (título, ISBN, año de publicación, etc.) y los organiza en una estructura de árbol general.
class ProcesarLibros {
public:
    // Procesa un libro individual y lo inserta en el árbol con sus datos.
    // Parámetros:
    //   - book: Elemento XML que contiene los datos del libro
    //   - tree: Referencia al árbol donde se insertarán los datos
    //   - parent: Nodo padre donde se agregará el libro
    //   - index: Índice del libro (para referencia numérica)
    void procesarLibro(tinyxml2::XMLElement* book, Tree& tree, Tree::Node* parent, int index);
    
    // Construye el árbol completo leyendo todos los archivos XML de la carpeta books_xml.
    // Crea la estructura raíz del árbol, itera sobre cada archivo XML en el directorio
    // y procesa cada libro de forma individual, agregándolo al árbol.
    // Retorna: Un árbol completamente construido con todos los libros y sus datos.
    static Tree construirArbol();

private:
    // Obtiene el texto de un elemento XML de forma segura.
    // Si el elemento es nullptr o no tiene contenido de texto, devuelve un string vacío
    // en lugar de causar un error. Esto previene accesos a memoria inválida.
    // Parámetro:
    //   - element: Elemento XML del cual extraer el texto
    // Retorna: El texto del elemento o un string vacío si no hay contenido
    static std::string safeText(tinyxml2::XMLElement* element);
    
    // Agrega un campo (etiqueta y valor) como nodo hijo en el árbol.
    // Crea un nodo para la etiqueta y luego un nodo hijo para el valor,
    // formando una estructura de dos niveles que representa un par clave-valor.
    // Parámetros:
    //   - tree: Referencia al árbol
    //   - parent: Nodo padre donde se agregará el campo
    //   - tag: Nombre de la etiqueta (clave)
    //   - value: Valor asociado a la etiqueta
    static void addCampo(Tree& tree, Tree::Node* parent, const std::string& tag, const std::string& value);
    
    // Procesa los libros similares de un libro y los inserta como subnodos.
    // Busca el elemento "similar_books" dentro de un libro, extrae cada libro similar,
    // y agrega su información (título, ISBN, año) al árbol como subnodos del libro principal.
    // Parámetros:
    //   - similarBooks: Elemento XML que contiene la lista de libros similares
    //   - tree: Referencia al árbol
    //   - parent: Nodo padre donde se insertarán los libros similares
    static void procesarSimilarBooks(tinyxml2::XMLElement* similarBooks, Tree& tree, Tree::Node* parent);
};

