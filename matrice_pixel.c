/*Auteur :JEAN-FRANCOIS BRANDON*/
#include "matrice_pixel.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

type_pixel ** init_matrice(image_info donnee_image){
    type_pixel **matrice;

    matrice = malloc(donnee_image.largeur * sizeof(*matrice));       //On alloue 'taille = largeur' pointeurs.
    if(matrice == NULL){
        fprintf(stderr,"error:[matrice] l'alllocation pour la matrice a failli \n");
        //png_read_end (png_ptr, NULL);
        //png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
        //fclose(fp);
        return NULL;
      //Ne pas oublier de notifier l'erreur et de quitter le programme.
    }
    for(int i=0 ; i < donnee_image.largeur ; i++){
        matrice[i] = malloc(donnee_image.hauteur * sizeof(**matrice) );       //On alloue des tableaux de 'taille = hauteur' variables.
        if(matrice[i] == NULL){  
            fprintf(stderr,"error:[matrice] l'alllocation pour la matrice a failli \n");
           // png_read_end (png_ptr, NULL);
           // png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
           //fclose(fp);
            free(matrice);
            return NULL;    
         }       
}                //En cas d'erreur d'allocation
         //Il faut libérer la mémoire déjà allouée
         //Ne pas oublier de notifier l'erreur et de quitter le programme.
       return matrice;
}

FILE* init_fichier(char *nom_du_fichier){
     png_byte signature[8];         // signature du fichier
    FILE * fp= fopen(nom_du_fichier, "rb"); // I/O initialisation
//verifie l'existence du fichier
    if (!fp){
        fprintf(stderr, "erreurr: ne peut pas ouvrir \"%s\"!\n",nom_du_fichier);
    }
//verifier si l'on traite une image png
//cela retourne faux si le bytes ne correspondent pas a la signature png, sinon vrai

    fread(signature, 1, sizeof(signature), fp);
    bool is_png = !png_sig_cmp(signature, 0, sizeof(signature));  //on passe les premeirs et huitieme bytes du fichier 
    if (!is_png)
    {
        fprintf(stderr,"erreur:\"%s\" n'est pas une image PNGF valide !\n",nom_du_fichier);
        fclose (fp);
        //return NULL;
    }
    return fp;
}

void correction_png_image(png_structp png_ptr,image_info donnee_image){

    if (donnee_image.type_de_couleur == PNG_COLOR_TYPE_PALETTE) {png_set_palette_to_rgb (png_ptr);}
    if (donnee_image.nbr_bit_pixel == 16){png_set_strip_16 (png_ptr);}
    else if (donnee_image.nbr_bit_pixel < 8){png_set_packing (png_ptr);}
}    

//void lecture_image(image_info ){}
void initialisation_lecture_image(png_structp png_ptr,png_infop info_ptr,png_infop end_info) {
    png_voidp user_error_ptr = 0;
    png_error_ptr user_error_fn = 0, user_warning_fn = 0;
//png _struct est une structure interne qui sera la pr0emeire variable a donne a chaque appel de fonctions de libpng
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)user_error_ptr, user_error_fn, user_warning_fn);//pointeur sur la structure de donnée
    if( !png_ptr){
        fprintf(stderr,"erreur:[read_png_file] png_create_read_struct a failli \n");
        //return NULL;
    }

    //png_info est une structure concue pour fournir les informations sur le fichier PNG
    info_ptr = png_create_info_struct(png_ptr); //pointeur optionnel pour les fonctions traitant les erreurs
    if(!info_ptr){
        fprintf(stderr,"error:[info_ptr] png_create_read_struct a failli \n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        //return NULL;
    }
    end_info = png_create_info_struct(png_ptr); // pointeur optionnel pour les fonctions traitant les erreurs
    if (!end_info)
    {   fprintf(stderr,"error:[end_info] png_create_read_struct a failli \n");
        png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
        //return NULL;
    }
}


image_info lecture_image_info(char *nom_du_fichier){
        
    FILE * fp =NULL;    
    image_info donnee_image;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    fp = init_fichier(nom_du_fichier);
    initialisation_lecture_image(png_ptr, info_ptr, end_info);
    
    png_init_io(png_ptr, fp); //Mettre en place libpng pour l'utilisation de la fonction standard C fread()  avec notre pointeur FILE 
    png_set_sig_bytes(png_ptr, 8); //indique a libpng qu'il y a des bytes qui manques depuis le debut du fichier

   /* lecture des info du png */
    png_read_info(png_ptr, info_ptr); 
 
    donnee_image.largeur = png_get_image_width(png_ptr, info_ptr);
    donnee_image.hauteur = png_get_image_height(png_ptr, info_ptr);
    donnee_image.type_de_couleur = png_get_color_type(png_ptr, info_ptr);
    donnee_image.nbr_bit_pixel = png_get_bit_depth(png_ptr, info_ptr);

    /* converti l'image en couleur RGB et redimensionne en 8 bytes   */
    correction_png_image(png_ptr, donnee_image);
    png_read_end (png_ptr, NULL);
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
    fclose(fp);
    
    return donnee_image;
   
}

type_pixel **lecture_image_pixel(char *nom_du_fichier,image_info donnee_image, type_pixel **matrice){
    png_infop end_info = NULL;
    png_structp png_ptr=NULL;
    png_infop info_ptr = NULL;
    png_bytep * ligne_pointeur;
    int x, y;

    FILE *fp=init_fichier(nom_du_fichier);
    donnee_image=lecture_image_info(nom_du_fichier);
   
    //lecture des pixel de l'image 
    
    ligne_pointeur = (png_bytep*) malloc(sizeof(png_bytep) * donnee_image.hauteur);
    for (y=0; y<donnee_image.hauteur; y++){
            ligne_pointeur[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
    }
    //png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_ALPHA, NULL);
    png_read_image(png_ptr, ligne_pointeur);
    //creation de la matrice de pixel
    for (y=0; y<donnee_image.hauteur; y++) {
                png_byte* ligne = ligne_pointeur[y];
                for (x=0; x<donnee_image.largeur; x++) {
                        png_byte* ptr = &(ligne[x*3]);
                        
                        matrice[x][y].R=ptr[0];
                        matrice[x][y].G=ptr[1];
                        matrice[x][y].B=ptr[2];
                }
        }
    free(ligne_pointeur);
    /* Finish decompression and release memory */
    png_read_end (png_ptr, NULL);
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return matrice;
}

