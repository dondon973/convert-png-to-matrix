
/*Auteur :JEAN-FRANCOIS BRANDON*/
#include "png.h"


typedef struct image_info image_info;
struct image_info 
{
    int largeur , hauteur ;  
     png_byte type_de_couleur;
     png_byte nbr_bit_pixel;
};
typedef struct type_pixel type_pixel;
struct type_pixel 
{
    int R,G,B;
};

type_pixel ** init_matrice(image_info donnee_image);
FILE* init_fichier(char *nom_du_fichier);
void correction_png_image(png_structp png_ptr,image_info donnee_image);
void initialisation_lecture_image(png_structp png_ptr,png_infop info_ptr,png_infop end_info);
image_info lecture_image_info(char *nom_du_fichier);
type_pixel **lecture_image_pixel(char *nom_du_fichier,image_info donnee_image, type_pixel **matrice);
