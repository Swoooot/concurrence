#ifndef _AREA_
#define _AREA_

	typedef enum status{
		FREE,OCCUPIED
	} status;

	typedef struct person{
		//Les coordonnées x et y correspondent au pixel haut gauche de la personne
		//Chaque personne occupe 4*4pixels
		int x;
		int y;
		int id;

	} person;

	typedef struct gridElement{
		status st;
		person p;

	} gridElement;

	
	
#endif
