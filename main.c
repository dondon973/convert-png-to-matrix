
#include "matrice_pixel.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

    


int main() {
    image_info data_image;
    char nom_du_fichier[] ="pngnow.png";
    type_pixel **tab = NULL;
    
    data_image = lecture_image_info(nom_du_fichier);
    
    tab = lecture_image_pixel(nom_du_fichier, data_image,tab);
   
}

