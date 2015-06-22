#include "MensajeLog.h"

MensajeLog::MensajeLog(): mensaje(std::string()) {
}

MensajeLog::~MensajeLog() {
}

void MensajeLog::agregar(std::string cadena) {
	mensaje << cadena;
}

void MensajeLog::agregar(int entero) {
	mensaje << entero;
}

void MensajeLog::agregar(double doble) {
	mensaje << doble;
}

std::string MensajeLog::getStringMensaje() {
	return mensaje.str();
}
