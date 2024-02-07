//Gutiérrez Segura Ian 1BV2

//-------------Utilerias
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//-------------Estructuras
struct PUNTO{
    int IdentificadorDePunto;
    float valorEnX;
    float valorEnY;
    float modulo;
    float angulo;
    int IdentificadorDeVecino;
};
struct DISTANCIA_MENOR{
    int IdentificadorDePunto1;
    int IdentificadorDePunto2;
    float distancia;
};

//-------------Prototipos
void menu_principal(int*);
void menu_secundario(int*);
void menu_detalles(int*);
void menu_operaciones(int*);
void menu_distancia(int*);
void llenar_PUNTOS(int,struct PUNTO*,unsigned int*,int);
float distancia_entre_puntos(float,float,float,float);
float angulo_de_puntos(float,float);
float validar_angulo(float);
void distancia_menor_vecino_cercano(struct PUNTO*,unsigned int,int*,struct DISTANCIA_MENOR*);

//-------------Funcion Principal
int main(){
	
	printf("\nPRÁCTICA FINAL DE PUNTOS\n");

    //Valores de la variable ronda:
    //0 -> Estamos en el menu_principal
    //1 -> Estamos en el menu_secundario
    //2 -> Estamos en algun otro menu
    int ronda = 0;   
    //Valores de la variable opc y opc2:
    //opc  -> opciones del menu principal y secundario
    //opc2 -> opciones de los otros menus
    int opc, opc2;
    //IdentificadorDePunto y cantidad total de puntos
    unsigned int id = 1, c_total = 0;
    //Arreglo de puntos
    struct PUNTO *PUNTOS;
    //Guarda los datos de la distancia menor
    struct DISTANCIA_MENOR d_menor;
    for(;;){
        //-----MENU
        if(ronda == 0){//0 -> Estamos en el menu_principal
            menu_principal(&opc);
            //----Lista aleatoria o manual
            if(opc==1 || opc==2){
                // c -> puntos que se quieren agregar
                int c;
                //Asignamos bloque de memoria
                PUNTOS = (struct PUNTO*)malloc(0);
                if(PUNTOS!=NULL){//Todo bien, seguimos:
                    do{
                        printf("\nCuantos puntos deseas crear?(0 si ya no deseas más puntos) ");
                        scanf("%d",&c);
                        __fpurge(stdin);
                        if(c==1 && c_total==0) printf("\nNo es aceptable su respuesta.\n");
                        else if(c>0){
                            //Reajustamos el bloque de memoria reservado
                            PUNTOS = (struct PUNTO*)realloc(PUNTOS,(c+c_total)*sizeof(struct PUNTO));
                            if(PUNTOS!=NULL){//Todo bien, seguimos:
                                //Llevamos el puntero hasta los lugares que aún no se llenan
                                PUNTOS += c_total;
                                //Agregamos los nuevos puntos
                                llenar_PUNTOS(c, PUNTOS, &id, opc);
                                //Regresamos el puntero al primer lugar
                                PUNTOS -= c_total;
                                //Actualizamos la variable "c_total"
                                c_total += c;
                                
                                //Pasamos a la siguiente ronda
                                ronda=1;
                                
                            }//Si hay error, repetimos menu.
                            else printf("\nError al asiganar el bloque de memoria dinamica\n");
                        }else if(c!=0) printf("\nNo es aceptable su respuesta.\n");
                    }while(c!=0);
                    if(c_total!=0){
                        //Abrimos el archivo en le que guardaremos los datos
                        FILE *f = fopen("compendioDePuntos.txt","w");
                        if(f!=NULL){
                            //Guardamos los detalles de cada punto 
                            for(c = 0; c < c_total; c++){
                                fprintf(f,"%d %f %f %f %f\n",PUNTOS->IdentificadorDePunto,PUNTOS->valorEnX,
                                PUNTOS->valorEnY,PUNTOS->modulo,PUNTOS->angulo);
                                PUNTOS++;
                            }
                            //Regresamos el puntero al primer lugar
                            PUNTOS -= c_total;
                            //Cerramos el archivo
                            fclose(f);
                        }//Si hay error, repetimos menu.
                        else printf("\nError al abrir archivo\n");
                    }
                }//Si hay error, repetimos menu.
                else printf("\nError al asiganar el bloque de memoria dinamica\n");
            }
            //----Lista archivo
            else if(opc==3){
                //Pedimos el nombre del archivo a leer
                char nom[21];
                printf("\nDame el nombre del archivo a leer: ");
                scanf("%s",nom);
                __fpurge(stdin);
                //Abrimos el archivo
                FILE *f = fopen(nom,"r");
                if(f!=NULL){
                    //Asignamos bloque de memoria
                    PUNTOS = (struct PUNTO*)malloc(0);
                    if(PUNTOS!=NULL){//Todo bien, seguimos:
                        //Mientras no lleguemos al final del archivo
                        while(!feof(f)){
                            c_total++;
                            //Reajustamos el bloque de memoria reservado
                            PUNTOS = (struct PUNTO*)realloc(PUNTOS, c_total*sizeof(struct PUNTO));
                            if(PUNTOS!=NULL){//Todo bien, seguimos:
                                //Llevamos el puntero hasta los lugares que aún no se llenan
                                PUNTOS += c_total-1;
                                //Obtenemos datos y guardamos
                                fscanf(f,"%d %f %f %f %f\n",&PUNTOS->IdentificadorDePunto,&PUNTOS->valorEnX,
                                &PUNTOS->valorEnY,&PUNTOS->modulo,&PUNTOS->angulo);
                                PUNTOS->IdentificadorDePunto = id++;
                                //Validamos que los puntos no superen el rango de -200 a 200
                                if(PUNTOS->valorEnX>200) PUNTOS->valorEnX=200;
                                else if(PUNTOS->valorEnX<-200) PUNTOS->valorEnX=-200;
                                if(PUNTOS->valorEnY>200) PUNTOS->valorEnX=200;
                                else if(PUNTOS->valorEnY<-200) PUNTOS->valorEnX=-200;
                                //Obtenemos el modulo y angulo con los decimales extendidos y valores positivos
                                PUNTOS->modulo = distancia_entre_puntos(PUNTOS->valorEnX,PUNTOS->valorEnY,0,0);
                                PUNTOS->angulo = angulo_de_puntos(PUNTOS->valorEnX,PUNTOS->valorEnY);
                                //Regresamos el puntero al inicio
                                PUNTOS -= c_total-1;
                                
                                //Pasamos a la siguiente ronda
                                ronda=1;
                                
                            }else{
                                printf("\nError al asiganar el bloque de memoria dinamica\n");
                            }
                        }
                    }//Si hay error, repetimos menu.
                    else printf("\nError al asiganar el bloque de memoria dinamica\n");
                    fclose(f);
                }//Si hay error, repetimos menu.
                else printf("\nError al abrir archivo\n");
            }
            //----SALIR
            else if(opc==4){
                //Liberamos el bloque de memoria dinamica
                free(PUNTOS);
                return 0;
            }
            //Creamos el archivo de la distancia menor entre puntos y de vecinos cercanos
            distancia_menor_vecino_cercano(PUNTOS,c_total,&ronda,&d_menor);
        }
        else if(ronda == 1){//1 -> Estamos en el menu_secundario
            menu_secundario(&opc);
            //Pasamos al siguiente menu
            ronda = 2;
        }
        else if(ronda == 2){//2 -> Estamos en algun otro menu
            //----Menu de Detalles
            if(opc==1){
                menu_detalles(&opc2);
                //---Un solo punto
                if(opc2==1){
                    //Obtenemos el id del punto a buscar
                    do{
                        printf("\nCual es el identificador del punto? ");
                        scanf("%d",&id);
                        __fpurge(stdin);
                    }while(id<1 || id>c_total);
                    //Hacemos que el puntero señale el punto dentro del bloque de memoria
                    PUNTOS+=id-1;
                    //Imprimimos los datos
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    //Regresamos el puntero al inicio del bloque de memoria
                    PUNTOS-=id-1;
                }
                //---Todos los puntos
                else if(opc2==2){
                    //Recorremos todos los espacios del bloque de memoria con aritmetica de punteros
                    for(int i = 0; i<c_total;i++){
                        //Imprimimos la info de cada punto
                        printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                        printf("\nValor en x: %f",PUNTOS->valorEnX);
                        printf("\nValor en y: %f",PUNTOS->valorEnY);
                        printf("\nModulo: %f",PUNTOS->modulo);
                        printf("\nAngulo: %f\n",PUNTOS->angulo);
                        PUNTOS++;
                    }
                    //Regresamos el puntero al inicio del bloque
                    PUNTOS-=c_total;
                }
                //Pasamos a la ronda anterior
                else if(opc2==3) ronda=1;
                //---SALIR
                else if(opc2==4){ 
                    //Liberamos el bloque de memoria dinamica
                    free(PUNTOS);
                    return 0;
                }
            }
            //----Menu de Operaciones
            else if(opc==2){
                menu_operaciones(&opc2);
                //---Suma o Resta o Multiplica o Divide
                if(opc2==1||opc2==2||opc2==3||opc2==4){
                    //Variable que almacena la cantidad de puntos a operar 
                    int _c;
                    do{
                        printf("\nDe cuantos puntos será la operación? ");
                        scanf("%d",&_c);
                        __fpurge(stdin);
                    }while(_c<1);
                    //Variables que guardan los resultados de la operacion suma y resta
                    float res_en_x = 0, res_en_y = 0;
                    //Variables que guardan los resultados de la operacion multiplica y divide
                    float modulo = 1, angulo = 0;
                    //Por la cantidad de puntos que operaremos, haremos:
                    for(int i = 0; i < _c; i++){
                        //Pedimos el identificador del punto (i+1)
                        do{
                            printf("\nCual es el identificador del punto %d? ",i+1);
                            scanf("%d",&id);
                            __fpurge(stdin);
                        }while(id<1 || id>c_total);
                        //Hacemos que el puntero seleccione al punto especificado
                        PUNTOS+=id-1;
                        //Si la operecion es una suma
                        if(opc2==1){
                            //Sumamos en X y Y
                            res_en_x += PUNTOS->valorEnX;
                            res_en_y += PUNTOS->valorEnY;
                        }else if(opc2==2){
                            //Si la operecion es una resta
                            if(i==0){
                                //El primer punto será positivo
                                res_en_x = PUNTOS->valorEnX;
                                res_en_y = PUNTOS->valorEnY;
                            }else{
                                //Los demas puntos seran negativos
                                res_en_x -= PUNTOS->valorEnX;
                                res_en_y -= PUNTOS->valorEnY;
                            }
                        }else if(opc2==3){
                            //Si la operecion es una multiplicación
                            //Se multiplican los modulos u se suman los angulos
                            modulo *= PUNTOS->modulo;
                            angulo += PUNTOS->angulo;
                            //Validamos que el angulo no supere los 360° o sea menor a 0°
                            angulo = validar_angulo(angulo);
                        }else if(opc2==4){
                            //Si la operecion es una división
                            if(i==0){
                                //El primer modulo será un dividendo
                                modulo = PUNTOS->modulo;
                                //El primer angulo será el positivo
                                angulo = PUNTOS->angulo;
                            }else{
                                //Los demás modulos serán los divisores
                                modulo /= PUNTOS->modulo;
                                //Los demás angulos serán los negativos
                                angulo -= PUNTOS->angulo;
                                //Validamos que el angulo no supere los 360° o sea menor a 0°
                                angulo = validar_angulo(angulo);
                            }
                        }
                        //Imprimimos los puntos que forman parte de la operación
                        printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                        printf("\nValor en x: %f",PUNTOS->valorEnX);
                        printf("\nValor en y: %f",PUNTOS->valorEnY);
                        printf("\nModulo: %f",PUNTOS->modulo);
                        printf("\nAngulo: %f\n",PUNTOS->angulo);
                        //Regresamos el puntero al inicio
                        PUNTOS-=id-1;
                    }
                    //Imprimimos resultados dependiendo de la operación
                    if(opc2==1||opc2==2)
                    printf("\nEl resultado de la operación es: (%f)+i(%f)\n",res_en_x,res_en_y);
                    if(opc2==3||opc2==4)
                    printf("\nEl resultado de la operación es: %f ∡%f°\n",modulo,angulo);
                }
                //Pasamos a la ronda anterior
                else if(opc2==5){ronda=1;}
                //---SALIR
                else if(opc2==6){
                    free(PUNTOS);
                    return 0;
                }
            }
            //----Distancias
            else if(opc==3){
                menu_distancia(&opc2);
                //---Entre dos puntos
                if(opc2==1){
                    //Pedimos el primer punto
                    do{
                        printf("\nCual es el identificador del punto 1? ");
                        scanf("%d",&id);
                        __fpurge(stdin);
                    }while(id<1 || id>c_total);
                    //Hacemos que el puntero señale el punto ingresado
                    PUNTOS+=id-1;
                    //Mostramos los datos del punto
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    //Obtenemos sus coordenadas
                    float ax = PUNTOS->valorEnX;
                    float ay = PUNTOS->valorEnY;
                    //Regresamos el puntero al inicio
                    PUNTOS-=id-1;
                    //Repetimos el proceso con el segundo punto
                    do{
                        printf("\nCual es el identificador del punto 2? ");
                        scanf("%d",&id);
                        __fpurge(stdin);
                    }while(id<1 || id>c_total);
                    PUNTOS+=id-1;
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    float bx = PUNTOS->valorEnX;
                    float by = PUNTOS->valorEnY;
                    PUNTOS-=id-1;
                    //Obtenemos la distancia entre los dos puntos
                    float d = distancia_entre_puntos(ax,ay,bx,by);
                    //Imprimimos resultado
                    printf("\nLa distancia entre estos puntos es de: %f\n",d);
                }
                //---La menor distancia
                else if(opc2==2){
                    //Imprimimos la distancia menor
                    printf("\nLa distancia menor (%f) es entre el punto %d y %d\n",
                    d_menor.distancia,d_menor.IdentificadorDePunto1,d_menor.IdentificadorDePunto2);
                    //Damos info de los dos puntos que cumplieron con dicha medición
                    PUNTOS+=d_menor.IdentificadorDePunto1-1;
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    PUNTOS-=d_menor.IdentificadorDePunto1-1;
                    PUNTOS+=d_menor.IdentificadorDePunto2-1;
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    PUNTOS-=d_menor.IdentificadorDePunto2-1;
                }
                //---Punto vecino
                else if(opc2==3){
                    //Pedimos el punto a evaluar
                    do{
                        printf("\nCual es el identificador del punto? ");
                        scanf("%d",&id);
                        __fpurge(stdin);
                    }while(id<1 || id>c_total);
                    //Imprimimos el punto seleccionado
                    PUNTOS+=id-1;
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    //Obtenemos el id, del punto vecino
                    int id_vecino = PUNTOS->IdentificadorDeVecino;
                    //Obtenemos las coordenadas
                    float ax = PUNTOS->valorEnX;
                    float ay = PUNTOS->valorEnY;
                    //Regresamos el puntero al inicio
                    PUNTOS-=id-1;
                    //Imprimimos el punto vecino
                    PUNTOS+=id_vecino-1;
                    printf("\nIdentificador: %d",PUNTOS->IdentificadorDePunto);
                    printf("\nValor en x: %f",PUNTOS->valorEnX);
                    printf("\nValor en y: %f",PUNTOS->valorEnY);
                    printf("\nModulo: %f",PUNTOS->modulo);
                    printf("\nAngulo: %f\n",PUNTOS->angulo);
                    //Obtenemos las coordenadas
                    float bx = PUNTOS->valorEnX;
                    float by = PUNTOS->valorEnY;
                    //Regresamos el puntero al inicio
                    PUNTOS-=id_vecino-1;
                    //Obtenemos la distancia:
                    float menor = distancia_entre_puntos(ax,ay,bx,by);
                    //Imprimimos en pantalla el resultado
                    printf("\nEl vecino mas cercano del punto %d es el punto %d con %f\n",id,id_vecino,menor);
                }
                //Pasamos a la ronda anterior
                else if(opc2==4){ronda=1;}
                //---SALIR
                else if(opc2==5){
                    //Liberamos los bloques de memoria reservados
                    free(PUNTOS);
                    return 0;
                }
            }
            //----Regresar
            else if(opc==4){
                //Regresamos las variables a los valores iniciales
                id = 1;
                c_total = 0;
                //Liberamos los bloques de memoria reservados
                free(PUNTOS);
                //Pasamos a la ronda anterior
                ronda=0;
            }
            //----SALIR
            else if(opc==5){
                //Liberamos los bloques de memoria reservados
                free(PUNTOS);
                return 0;
            }
        }
    }
}

//-------------Funciones
void menu_principal(int *opc){
    do{
    printf("\nMenú Principal\n");
    printf("1.- Genera lista de puntos aleatoriamente\n");
    printf("2.- Genera lista de puntos manualmente\n");
    printf("3.- Obtener puntos de archivo\n");
    printf("4.- Salir\n");
    printf("Opción: ");
    scanf("%d",opc);
    __fpurge(stdin);
    }while(*opc!=1&*opc!=2&*opc!=3&*opc!=4);
    return;
}
void menu_secundario(int *opc){
    do{
    printf("\nSegundo Menú\n");
    printf("1.- Detalles de puntos\n");
    printf("2.- Operaciones entre puntos\n");
    printf("3.- Distancias entre puntos\n");
    printf("4.- Regresar(Se perderan los datos guardados)\n");
    printf("5.- Salir\n");
    printf("Opción: ");
    scanf("%d",opc);
    __fpurge(stdin);
    }while(*opc!=1&*opc!=2&*opc!=3&*opc!=4&*opc!=5);
    return;
}
void menu_detalles(int *opc){
    do{
    printf("\nMenú de Detalles\n");
    printf("1.- Detalles de un punto\n");
    printf("2.- Detalles de todos los puntos\n");
    printf("3.- Regresar\n");
    printf("4.- Salir\n");
    printf("Opción: ");
    scanf("%d",opc);
    __fpurge(stdin);
    }while(*opc!=1&*opc!=2&*opc!=3&*opc!=4);
    return;
}
void menu_operaciones(int *opc){
    do{
    printf("\nMenú de Operaciones\n");
    printf("1.- Suma puntos\n");
    printf("2.- Resta puntos\n");
    printf("3.- Multiplica puntos\n");
    printf("4.- Divide puntos\n");
    printf("5.- Regresar\n");
    printf("6.- Salir\n");
    printf("Opción: ");
    scanf("%d",opc);
    __fpurge(stdin);
    }while(*opc!=1&*opc!=2&*opc!=3&*opc!=4&*opc!=5&*opc!=6);
    return;
}
void menu_distancia(int *opc){
    do{
    printf("\nMenú de Distancia\n");
    printf("1.- Determina distancia entre puntos\n");
    printf("2.- Determina la menor distancia entre puntos\n");
    printf("3.- Determinar el punto vecino más cercano\n");
    printf("4.- Regresar\n");
    printf("5.- Salir\n");
    printf("Opción: ");
    scanf("%d",opc);
    __fpurge(stdin);
    }while(*opc!=1&*opc!=2&*opc!=3&*opc!=4&*opc!=5);
    return;
}
void llenar_PUNTOS(int c, struct PUNTO *PUNTOS, unsigned int *id, int mod){
    //llenamos el arreglo con aritmetica de punteros
    srand (time(NULL));
    for(int i = 0; i < c; i++){
        
        //Guardamos el id
        PUNTOS -> IdentificadorDePunto = *id;
        //Imprimimos el id
        printf("\nIdentificador: %d\n",*id);
        //Le aumentamos uno
        *id+=1;
        
        if(mod==1){//LLenado aleatorio
            //Numeros aleatorios entre el -200 al 200, con decimales
            PUNTOS -> valorEnX = ((rand()%399)-200)+(float)(rand()%1000)/1000;
            PUNTOS -> valorEnY = ((rand()%399)-200)+(float)(rand()%1000)/1000;
            //Imprimimos las coordenadas
            printf("Valor en x: %f",PUNTOS -> valorEnX);
            printf("\nValor en y: %f\n",PUNTOS -> valorEnY);
        }
        else if(mod==2){//LLenado manual
            //Pedimos coordena x entre -200 y 200
            do{
                printf("Ingresa el valor en x entre -200 y 200: ");
                scanf("%f",& PUNTOS -> valorEnX);
                __fpurge(stdin);
            }while(PUNTOS->valorEnX>200||PUNTOS->valorEnX<-200);
            //Pedimos coordena y entre -200 y 200
            do{
                printf("Ingresa el valor en y entre -200 y 200: ");
                scanf("%f",& PUNTOS -> valorEnY);
                __fpurge(stdin);
            }while(PUNTOS->valorEnY>200||PUNTOS->valorEnY<-200);
        }
        //Calculamos y guardamos el modulo del punto
        PUNTOS->modulo = distancia_entre_puntos(PUNTOS->valorEnX,PUNTOS->valorEnY,0,0);
        //Calculamos y guardamos el angulo del punto
        PUNTOS->angulo = angulo_de_puntos(PUNTOS->valorEnX,PUNTOS->valorEnY);
        
        //Imprimimos los valores
        printf("Modulo: %f",PUNTOS->modulo);
        printf("\nAngulo: %f\n",PUNTOS->angulo);
        
        //Pasamos al siguiente punto
        PUNTOS++;
    }
    return;
}
float distancia_entre_puntos(float ax,float ay,float bx,float by){
    //Obtenemos la distancia en el eje_x y en el eje_y
    float distancia_x = ax-bx;
    float distancia_y = ay-by;
    //La distancia entre puntos es la raiz de la distancia en "x" al cuadrado más la de "y" al cuadrado
    float distancia = sqrt(pow(distancia_x,2)+pow(distancia_y,2));
    
    return distancia;
}
float angulo_de_puntos(float ax,float ay){
    //Existen algunas excepciones a la formula que debemos evaluar
    //Como son los 0°, 90° y 270°
    if(ax==0){
        if(ay<0) return 270;
        else if(ay==0) return 0;
        else return 90;
    }
    //La tangente del angulo es igual a cateto opuesto sobre cateto adyacente
    //Por lo que depejamos al angulo de esta formula
    float angulo_radianes = atan(ay/ax);
    //1rad = 180/pi
    float angulo_grados = angulo_radianes*180/3.14159265358979323846/*valor de pi*/;
    //Ahora, tenemos que identificar el cuadrante en el que se encuentra para que el angulo en grados
    //este bien representado en cordenadas polares con angulo positivo
    if(ax < 0)angulo_grados = 180+angulo_grados;
    else if(ay<0)angulo_grados = 360+angulo_grados;
    
    return angulo_grados;
}
float validar_angulo(float angulo){
    //Restamos o sumamos, dependera del caso, hasta que el angulo este entre el 0° y los 360°
    while(angulo <= 0){
        angulo += 360;
    }
    while(angulo >= 360){
        angulo -= 360;
    }
    return angulo;
}
void distancia_menor_vecino_cercano(struct PUNTO *PUNTOS,unsigned int c_total,int *ronda,struct DISTANCIA_MENOR *d_menor){
    if(c_total==0){
        //Repetimos ronda
        *ronda = 0;
        return;
    }
    //-----Distancia Menor
    //Abrimos nuestro archivo
    FILE *f = fopen("resultadosDeDistancias.txt","w");
    //-----Vecino cercano
    //Abrimos nuestro archivo
    FILE *f2 = fopen("vecinoMasCercano.txt","w");
    if(f!=NULL && f2!=NULL){
        //menor -> Guardara la menor distancia
        //menor2 -> Guardara la menor distancia entre vecinos
        float menor, menor2;
        //Guardaran el identificador del primer y segundo punto
        int id1, id11;
        int id2, id22;
        //Titulo
        fprintf(f,"\tDistancias entre puntos\n\n");
        //Titulo
        fprintf(f2,"\tVecino cercano entre puntos\n\n");
        //Ponemos menor a un valor muy grande
            menor = 1000;
        //Pasamos por cada punto en el bloque de memoria
        for(int i = 0; i<c_total; i++){
            //Ponemos menor2 a un valor muy grande
            menor2 = 1000;
            //Guardamos el id del primer punto
            id11 = i+1;
            //Obtenemos las coordenadas del punto
            PUNTOS+=i;
            float ax = PUNTOS->valorEnX;
            float ay = PUNTOS->valorEnY;
            PUNTOS-=i;
            if(i!=c_total-1){
                //Pasamos por cada punto en el bloque de memoria a partir del que se tiene en i
                for(int j = i+1; j<c_total; j++){
                    //Obtenemos las coordenadas del punto
                    PUNTOS+=j;
                    float bx = PUNTOS->valorEnX;
                    float by = PUNTOS->valorEnY;
                    PUNTOS-=j;
                    //Calculamos la distancia entre ambos puntos
                    float d = distancia_entre_puntos(ax,ay,bx,by);
                    //veremos si d es menor a la que teniamos guardada
                    if(menor > d){
                        //En tal caso, actualizamos los datos
                        menor = d;
                        id1 = i+1;
                        id2 = j+1;
                    }
                    //Imprimimos en el archivo la distancia y los puntos de cada comparación 
                    fprintf(f,"Punto1: %d\tPunto2: %d\tDistancia: %f\n",i+1,j+1,d);
                }
            }
            //Volvemos a pasamos por cada punto en el bloque de memoria
            for(int j = 0; j<c_total; j++){
                //Mientrs no sea el mismo punto
                if(i!=j){
                    //Obtenemos las coordenadas del punto
                    PUNTOS+=j;
                    float bx = PUNTOS->valorEnX;
                    float by = PUNTOS->valorEnY;
                    PUNTOS-=j;
                    //Calculamos la distancia entre ambos puntos
                    float d = distancia_entre_puntos(ax,ay,bx,by);
                    
                    //Veremos si d es menor a la que teniamos guardada
                    if(menor2 > d){
                        //En tal caso, actualizamos los datos
                        menor2 = d;
                        //Guardamos el id del segundo punto
                        id22 = j+1;
                    }
                }
            }
            //Imprimimos en el archivo la distancia menor y los puntos de cada comparación 
            fprintf(f2,"Punto1: %d\tPunto2: %d\tDistancia: %f\n",id11,id22,menor2);
            PUNTOS+=i;
            PUNTOS->IdentificadorDeVecino = id22;
            PUNTOS-=i;
        }
        //Guardo los datos importantes de la distancia menor entre puntos
        d_menor -> IdentificadorDePunto1 = id1;
        d_menor -> IdentificadorDePunto2 = id2;
        d_menor -> distancia = menor;
        //Cerramos los archivo
        fclose(f);
        fclose(f2);
        //Pasaremos a la siguiente ronda
        *ronda = 1;
        return;
    }
	//En caso de un problema
    printf("\nHubo un problema al crear los archivos.\n");
    //Repetimos ronda
    *ronda = 0;
    return;
}