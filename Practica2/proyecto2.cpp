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
        cout << "Codigo: ";
        cin >> nuevo.codigo;
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

    cout << "Nombre: ";
    cin.getline(nuevo.nombre, 30);
    cout << "Precio: ";
    cin >> nuevo.precio;
    cout << "Stock: ";
    cin >> nuevo.stock;
    cin.ignore();
    cout << "Categoria: ";
    cin.getline(nuevo.categoria, 20);

    nuevo.activo = true;
    ofstream out(archivo, ios::binary | ios::app);
    out.write((char*)&nuevo, sizeof(Producto));
    out.close();

    cout << "Producto agregado!\n";
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
