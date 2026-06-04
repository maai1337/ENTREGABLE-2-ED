#include "tree.hpp"
#include <algorithm>
#include <functional>

// Node
Tree::Node::Node(const std::string& value, Node* p) {
    data = value;
    parent = p;
}

// Constructor
Tree::Tree(int k) {
    this->k = k;
    rootNode = nullptr;
    treeSize = 0;
}

bool Tree::isEmpty() {
    return treeSize == 0;
}

int Tree::size() {
    return treeSize;
}

std::string Tree::root() {
    if (!rootNode) throw std::runtime_error("Árbol vacío");
    return rootNode->data;
}

Tree::Node* Tree::rootNodePtr() {
    return rootNode;
}

Tree::Node* Tree::insertRoot(const std::string& value) {
    if (!rootNode) {
        rootNode = new Node(value);
        treeSize++;
    }
    return rootNode;
}

Tree::Node* Tree::insertChild(Node* parent, const std::string& value) {
    if (!parent) return nullptr;
    if ((int)parent->children.size() >= k) return nullptr;

    Node* newNode = new Node(value, parent);
    parent->children.push_back(newNode);
    treeSize++;
    return newNode;
}

Tree::Node* Tree::search(Node* node, const std::string& value) {
    if (!node) return nullptr;
    if (node->data == value) return node;

    for (auto child : node->children) {
        Node* found = search(child, value);
        if (found) return found;
    }
    return nullptr;
}

bool Tree::insert(const std::string& parentValue, const std::string& value) {
    if (!rootNode) {
        rootNode = new Node(value);
        treeSize++;
        return true;
    }

    Node* parentNode = search(rootNode, parentValue);
    if (!parentNode) return false;

    if ((int)parentNode->children.size() >= k) return false;

    Node* newNode = new Node(value, parentNode);
    parentNode->children.push_back(newNode);
    treeSize++;
    return true;
}

std::string Tree::parent(const std::string& value) {
    Node* node = search(rootNode, value);
    if (!node || !node->parent)
        throw std::runtime_error("No tiene padre");

    return node->parent->data;
}

std::vector<std::string> Tree::children(const std::string& value) {
    Node* node = search(rootNode, value);
    std::vector<std::string> result;

    if (!node) return result;

    for (auto child : node->children)
        result.push_back(child->data);

    return result;
}

void Tree::deleteSubtree(Node* node) {
    if (!node) return;
    for (auto child : node->children)
        deleteSubtree(child);
    delete node;
	treeSize--;
}

bool Tree::remove(const std::string& value) {
    Node* node = search(rootNode, value);
    if (!node) return false;

    if (node == rootNode) {
        deleteSubtree(rootNode);
        rootNode = nullptr;
        treeSize = 0;
        return true;
    }

    Node* parent = node->parent;
    auto& siblings = parent->children;

    siblings.erase(
        std::remove(siblings.begin(), siblings.end(), node),
        siblings.end()
    );

    deleteSubtree(node);
    treeSize--;
    return true;
}


void Tree::preOrder(Node* node, std::vector<std::string>& result) {
    if (!node) return;
    result.push_back(node->data);
    for (auto child : node->children)
        preOrder(child, result);
}

std::vector<std::string> Tree::preOrder() {
    std::vector<std::string> result;
    preOrder(rootNode, result);
    return result;
}

void Tree::postOrder(Node* node, std::vector<std::string>& result) {
    if (!node) return;
    for (auto child : node->children)
        postOrder(child, result);
    result.push_back(node->data);
}

std::vector<std::string> Tree::postOrder() {
    std::vector<std::string> result;
    postOrder(rootNode, result);
    return result;
}

std::vector<std::string> Tree::inOrder() {
    std::vector<std::string> result;

    std::function<void(Node*)> inorder = [&](Node* node) {
        if (!node) return;

        int half = node->children.size() / 2;

        for (int i = 0; i < half; i++)
            inorder(node->children[i]);

        result.push_back(node->data);

        for (size_t i = half; i < node->children.size(); i++)
            inorder(node->children[i]);
    };

    inorder(rootNode);
    return result;
}

// Funcion que ayuda a buscar un tag/etiqueta (como "id" o "publication_year")
// dentro de un nodo de libro y devolver el texto que contiene.
std::string Tree::obtenerValorEtiqueta(Node* bookNode, const std::string& tag) {
    for (auto child : bookNode->children) {
        if (child->data == tag && !child->children.empty()) {
            return child->children[0]->data;
        }
    }
    return "";
}

// Esta funcion usa recursion y es para recorrer el arbol en pre-order
void Tree::listar_preorder(Node* node) {
    if (!node) return;

    if (node->data == "book") {
        std::string id = obtenerValorEtiqueta(node, "id");
        if (!id.empty()) {
            std::cout << "ID: " << id << "\n";
        }
    }

    for (auto child : node->children) {
        listar_preorder(child);
    }
}

void Tree::listar() {
    listar_preorder(rootNode);
}

// Revisa cada nodo recursivamente para ver si cumple la condicion de ser precursor
void Tree::precursores_recursivo(Node* node) {
    if (!node) return;

    if (node->data == "book") {
        std::string idStr = obtenerValorEtiqueta(node, "id");
        std::string yearStr = obtenerValorEtiqueta(node, "publication_year");

		// Este if revisa que el libro tenga id y año de publicacion validos
        if (!idStr.empty() && !yearStr.empty()) {
            try {
                int mainYear = std::stoi(yearStr);
                bool cumpleCondicion = true;
                bool tieneSimilares = false;

				// Se busca el nodo que contenga libros similares
                Node* similarNode = nullptr;
                for (auto c : node->children) {
                    if (c->data == "similar_books") {
                        similarNode = c;
                        break;
                    }
                }

                if (similarNode) {
					// Aqui se revisa cada libro similar dentro de la lista
                    for (auto simBook : similarNode->children) {
                        if (simBook->data == "book") {
                            tieneSimilares = true;
                            std::string simYearStr = obtenerValorEtiqueta(simBook, "publication_year");
                            if (!simYearStr.empty()) {
                                int simYear = std::stoi(simYearStr);
								// Este if hace que la condicion falle si un libro similar salio el mismo año o antes
                                if (simYear <= mainYear) {
                                    cumpleCondicion = false;
                                    break;
                                }
                            }
                        }
                    }
                }
				// Si tiene libros similares y todos los similares salieron despues, entonces es precursor
                if (tieneSimilares && cumpleCondicion) {
                    std::cout << "ID Precursor: " << idStr << "\n";
                }
            } catch (...) {}
        }
    }
	// Continua recorriendo el arbol
    for (auto child : node->children) {
        precursores_recursivo(child);
    }
}

void Tree::precursores() {
    precursores_recursivo(rootNode);
}

// Funcion que elimina del arbol todos los libros con un rating menor o igual a r
void Tree::borrar_ratings(float r) {
    if (!rootNode) return;

	// Busca el nodo principal books que tiene a todos los libros
    Node* booksNode = nullptr;
    for (auto child : rootNode->children) {
        if (child->data == "books") {
            booksNode = child;
            break;
        }
    }

    if (!booksNode) return;

	// Recorre la lista de libros de atras a adelante, asi evita saltarse elementos al borrar nodos del vector
    for (int i = (int)booksNode->children.size() - 1; i >= 0; i--) {
        Node* bookNode = booksNode->children[i];

        if (bookNode->data == "book") {
            std::string ratingStr = obtenerValorEtiqueta(bookNode, "average_rating");

            if (!ratingStr.empty()) {
                try {
                    float rating = std::stof(ratingStr);
					// Si el rating es menor o igual a r, se elimina
                    if (rating <= r) {
                        deleteSubtree(bookNode);

                        booksNode->children.erase(booksNode->children.begin() + i);
                    }
                } catch (...) {}
            }
        }
    }
}