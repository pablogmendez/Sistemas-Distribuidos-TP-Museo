#include "SignalHandler.h"

SignalHandler* SignalHandler :: instance = NULL;
EventHandler* SignalHandler :: signal_handlers [ NSIG ];

SignalHandler :: SignalHandler () {
}

SignalHandler* SignalHandler :: getInstance () {

	if ( instance == NULL )
		instance = new SignalHandler ();

	return instance;
}

void SignalHandler :: destruir () {
	if ( instance != NULL ) {
		delete ( instance );
		instance = NULL;
	}
}

EventHandler* SignalHandler :: registrarHandler ( int signum,EventHandler* eh ) {

	EventHandler* old_eh = SignalHandler :: signal_handlers [ signum ];
	SignalHandler :: signal_handlers [ signum ] = eh;

	struct sigaction sa;
	sa.sa_handler = SignalHandler :: dispatcher;
	sigemptyset ( &sa.sa_mask );	// inicializa la mascara de seniales a bloquear durante la ejecucion del handler como vacio
	sigaddset ( &sa.sa_mask,signum );
	sigaction ( signum,&sa,0 );	// cambiar accion de la senial

	return old_eh;
}

void SignalHandler :: dispatcher ( int signum ) {

	if ( SignalHandler :: signal_handlers [ signum ] != 0 )
		SignalHandler :: signal_handlers [ signum ]->handleSignal ( signum );
}

int SignalHandler :: removerHandler ( int signum ) {

	SignalHandler :: signal_handlers [ signum ] = NULL;
	return 0;
}

EventHandler* SignalHandler::ignorar (int signum, int flags)
{
	EventHandler* old_eh = SignalHandler::signal_handlers[signum];
	SignalHandler::signal_handlers[signum] = NULL;

	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset (&sa.sa_mask);
	sigaddset (&sa.sa_mask, signum);
	sa.sa_flags = flags;
	sigaction (signum, &sa, 0);
	return old_eh;
}
