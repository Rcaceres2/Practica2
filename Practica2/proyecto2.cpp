#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct Producto {
    int codigo, stock;
    char nombre[30], categoria[30];
    float precio;
    bool activo;
};

const char* archivo = "inventario.dat";

void inicializar() {
    ofstream file(archivo, ios::binary | ios::app);
    file.close();
}

void agregarProducto() {
    Producto nuevo;
    bool codigoRepetido;
    
    do {
        codigoRepetido = false;
        cout << "Codigo: "; cin >> nuevo.codigo;
        cin.ignore();

        ifstream file(archivo, ios::binary);
        Producto p;
        while(file.read((char*)&p, sizeof(Producto))) {
            if(p.codigo == nuevo.codigo && p.activo) {
                cout << "Codigo ya registrado\n";
                codigoRepetido = true;
                break;
            }
        }
        file.close();
    } while(codigoRepetido);

    cout << "Nombre: "; cin.getline(nuevo.nombre, 30);
    cout << "Precio: "; cin >> nuevo.precio;
    cout << "Stock: "; cin >> nuevo.stock;
    cin.ignore();
    cout << "Categoria: "; cin.getline(nuevo.categoria, 30);

    nuevo.activo = true;
    ofstream out(archivo, ios::binary | ios::app);
    out.write((char*)&nuevo, sizeof(Producto));
    out.close();

    cout << "Producto agregado!\n";
}

void mostrarTodos() {
    ifstream file(archivo, ios::binary);
    if(!file) {
        cout << "No hay productos\n";
        return;
    }
    Producto p;
    bool hayProductos = false;
    
    cout << "\nLista de productos\n";
    while(file.read((char*)&p, sizeof(Producto))) {
        if(p.activo) {
            hayProductos = true;
            cout << p.codigo << " | " << p.nombre << " | $" << p.precio << " | Stock: " << p.stock << " | " << p.categoria << "\n";
        }
    }
    file.close();
    if(!hayProductos) cout << "No hay productos activos\n";
}

void mostrarPorCategoria() {
    string categoria;
    cout << "Categoria a buscar: "; getline(cin, categoria);

    ifstream file(archivo, ios::binary);
    if(!file) {
        cout << "Error al abrir archivo\n";
        return;
    }

    Producto p;
    bool encontrados = false;
    
    cout << "\nProductos en " << categoria << ":\n";
    while(file.read((char*)&p, sizeof(Producto))) {
        if(p.activo && string(p.categoria) == categoria) {
            encontrados = true;
            cout << p.codigo << " | " << p.nombre << " | $" << p.precio << " | Stock: " << p.stock << "\n";
        }
    }
    file.close();
    if(!encontrados) cout << "No hay productos en esta categoria\n";
}

void buscarPorCodigo() {
    int codigo;
    cout << "Codigo a buscar: "; cin >> codigo;
    cin.ignore();

    ifstream file(archivo, ios::binary);
    if(!file) {
        cout << "Error al abrir archivo\n";
        return;
    }
    Producto p;
    bool encontrado = false;
    
    while(file.read((char*)&p, sizeof(Producto))) {
        if(p.codigo == codigo && p.activo) {
            encontrado = true;
            cout << "\nCodigo: " << p.codigo << "\nNombre: " << p.nombre << "\nPrecio: $" << p.precio << "\nStock: " << p.stock << "\nCategoria: " << p.categoria << "\n";
            break;
        }
    }
    file.close();
    if(!encontrado) cout << "Producto no encontrado\n";
}

void modificarProducto() {
    int codigo;
    cout << "Codigo a modificar: "; cin >> codigo;
    cin.ignore();

    fstream file(archivo, ios::binary | ios::in | ios::out);
    if(!file) {
        cout << "Error al abrir archivo\n";
        return;
    }
    Producto p;
    bool encontrado = false;
    streampos pos;

    while(file.read((char*)&p, sizeof(Producto))) {
        if(p.codigo == codigo && p.activo) {
            encontrado = true;
            pos = file.tellg();
            pos -= sizeof(Producto);
            break;
        }
    }
    if(!encontrado) {
        cout << "Producto no encontrado\n";
        file.close();
        return;
    }
    cout << "\n1. Nombre: " << p.nombre
         << "\n2. Precio: " << p.precio
         << "\n3. Stock: " << p.stock
         << "\n4. Categoria: " << p.categoria
         << "\nQue desea modificar? (1-4): ";
    
    int opcion;
    cin >> opcion;
    cin.ignore();

    switch(opcion) {
        case 1: cout << "Nuevo nombre: "; cin.getline(p.nombre, 30); break;
        case 2: cout << "Nuevo precio: "; cin >> p.precio;
            cin.ignore();
            break;
        case 3: cout << "Nuevo stock: "; cin >> p.stock;
            cin.ignore();
            break;
        case 4: cout << "Nueva categoria: "; cin.getline(p.categoria, 30);
            break;
        default: cout << "Opcion no valida\n";
            file.close();
            return;
    }
    file.seekp(pos);
    file.write((char*)&p, sizeof(Producto));
    file.close();
    
    cout << "Producto modificado!\n";
}

void eliminarProducto() {
    int codigo;
    cout << "Codigo a eliminar: "; cin >> codigo;
    cin.ignore();

    fstream file(archivo, ios::binary | ios::in | ios::out);
    if(!file) {
        cout << "Error al abrir archivo\n";
        return;
    }
    Producto p;
    bool encontrado = false;
    streampos pos;

    while(file.read((char*)&p, sizeof(Producto))) {
        if(p.codigo == codigo && p.activo) {
            encontrado = true;
            pos = file.tellg();
            pos -= sizeof(Producto);
            break;
        }
    }
    if(!encontrado) {
        cout << "Producto no encontrado\n";
        file.close();
        return;
    }
    p.activo = false;
    file.seekp(pos);
    file.write((char*)&p, sizeof(Producto));
    file.close();
    
    cout << "Producto eliminado!\n";
}

int main() {
    inicializar();
    int opcion;

    do {
        cout << "1. Agregar\n"
            << "2. Mostrar todos\n"
            << "3. Buscar por categoria\n"
            << "4. Buscar por codigo\n"
            << "5. Modificar\n"
            << "6. Eliminar\n"
            << "7. Salir\n"
            << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: agregarProducto(); break;
            case 2: mostrarTodos(); break;
            case 3: mostrarPorCategoria(); break;
            case 4: buscarPorCodigo(); break;
            case 5: modificarProducto(); break;
            case 6: eliminarProducto(); break;
            case 7: cout << "Saliendo...\n"; break;
            default: cout << "Opcion no valida\n";
        }
    } while(opcion != 7);
    return 0;
}
