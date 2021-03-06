//Nicholas Ribeiro Roder R.A.:1583174
#include "pdi.h"
#include <math.h>

PDI::PDI(){

}

PDI::~PDI(){

}


void PDI::carregarImagem(String filename){
    //le a imagem
    imagemRGB = imread(filename,1);
    //caso a imagem tenha sido lida com sucesso
    if(imagemRGB.data){
    //exibe a imagem na janela
    imshow("Imagem Original", imagemRGB);
    }
    //CONVERSAO PARA ESCALA DE CINZA
    cvtColor(imagemRGB,imagemCINZA,COLOR_BGR2GRAY);
    //exibe a imagem cinza na janela
    imshow("Imagem Cinza", imagemCINZA);

}

void PDI::negativo(){
     Mat negativo = imagemCINZA.clone();
     for(int x=0;x<negativo.rows;x++){
         for(int y=0;y<negativo.cols;y++){
             uchar pixel = negativo.at<uchar>(x,y);
             negativo.at<uchar>(x,y) = 255-pixel;
         }
     }
     //exibe a imagem na janela
     imshow("Negativo de uma Imagem", negativo);
}

void PDI::ajusteContraste(){
     Mat contraste = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<contraste.rows;x++){
         for(int y=0;y<contraste.cols;y++){
             uchar pixel = contraste.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }
     //g=((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin
     uchar gmax=255; uchar gmin=0;
     for(int x=0;x<contraste.rows;x++){
         for(int y=0;y<contraste.cols;y++){
             uchar f = contraste.at<uchar>(x,y);
             contraste.at<uchar>(x,y) = ((gmax-gmin)/(fmax-fmin))*(f-fmin)+gmin;
         }
     }
     //exibe a imagem na janela
     imshow("Imagem Contraste", contraste);
}

void PDI::histograma(){
    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    int h[256];
    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;
    //calcula a distribuicao dos niveis de cinza
    int nivel = 0;
    for(int x=0;x<imagemCINZA.rows;x++){
        for(int y=0;y<imagemCINZA.cols;y++){
            nivel = (int)imagemCINZA.at<uchar>(x,y);
            h[nivel] += 1;
        }
    }

    //normalizar vetor para plotar o grafico
    int maior=0;
    for(int i=0;i<256;i++){
        if(h[i]>maior) maior=h[i];
    }
    int hn[256];
    for(int i=0;i<256;i++){
        hn[i]=round(h[i]*255/maior);
    }

    //criar imagem em branco para o histrograma
    int altura=256; int largura=512;
    Mat imagemH(altura,largura,CV_8UC3,Scalar(255,255,255));
    Point pt1 = Point(0,0); Point pt2 = Point(0,0);
    for(int i=0; i<altura; i++){
        pt1.x=i*2;pt1.y=altura-1;
        pt2.x=i*2;pt2.y=pt1.y-hn[i];
        line(imagemH,pt1,pt2,Scalar(255,100,50+hn[i]),1,8);
    }

    //exibe a imagem na janela
    imshow("Histograma", imagemH);
}

void PDI::logaritmica(){
     Mat logImage = imagemCINZA.clone();
     //fmax fmin
     uchar fmax=0; uchar fmin=255;
     for(int x=0;x<logImage.rows;x++){
         for(int y=0;y<logImage.cols;y++){
             uchar pixel = logImage.at<uchar>(x,y);
             if(pixel > fmax) fmax = pixel;
             if(pixel < fmin) fmin = pixel;
         }
     }

     for(int x=0;x<logImage.rows;x++){
         for(int y=0;y<logImage.cols;y++){
             uchar f = logImage.at<uchar>(x,y);
             double a = 255/log(1+fmax);
             double result = a*log(f+1);
             logImage.at<uchar>(x,y) = (uchar)result;
         }
     }

     //exibe a imagem na janela
     imshow("Imagem Log", logImage);
}

void PDI::potencia(){
     Mat p = imagemCINZA.clone();
     for(int x=0;x<p.rows;x++){
         for(int y=0;y<p.cols;y++){
             uchar f = p.at<uchar>(x,y);
             double a = 2; double c=1;
             double result = a*pow(f,c);
             p.at<uchar>(x,y) = (uchar)result;
         }
     }

     //exibe a imagem na janela
     imshow("Transf de Potencia ", p);
}

void PDI::equalizacaoHistograma(){

    Mat equal = imagemCINZA.clone();
    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    float h[256];

    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;

    //calcula a distribuicao dos niveis de cinza
    for(int x=0;x<equal.rows;x++){
        for(int y=0;y<equal.cols;y++){
            h[(int)equal.at<uchar>(x,y)] += 1;
        }
    }

    //vetor do histograma normalizado
    float tamanho = equal.rows * equal.cols;
    for(int i=0;i<256;i++){
        h[i]=h[i]/tamanho;
    }

    //vetor do histograma normalizado acumulado
    for(int x=1; x<256; x++){
        h[x] = h[x]+h[x-1];
    }

    //calculo do histograma equalizado
    for(int x=0; x<256; x++){
        h[x] = round((255)*h[x]);
    }

    //imagem final
    for(int x=0;x<equal.rows;x++){
        for(int y=0;y<equal.cols;y++){
            equal.at<uchar>(x,y) = (int)h[(int)equal.at<uchar>(x,y)];
        }
    }

    imshow("Imagem equalizada", equal);
}

int maxmin(int vet[], bool x){

    // funcao para encontrar o maximo ou minimo de um vetor
    if(!x){
        int maximo = 0;

        for(int i=0; i<9; i++){
            if(vet[i] > maximo){
                maximo = vet[i];
            }
        }

        return maximo;
    }else{
        int minimo = 255;

        for(int i=0; i<9; i++){
            if(vet[i] < minimo){
                minimo = vet[i];
            }
        }

        return minimo;
    }
}

void PDI::filtroMaximo(){
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();

    //percorre a imagem inteira
    for(int x=0;x<f.rows;x++){
        for(int y=0;y<f.cols;y++){
            int m[9];
            int k=0;
            //percorre uma mascara 3x3
            for(int i=-1; i<2; i++){
                for(int j=-1; j<2; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) m[k] = f.at<uchar>(x-i,y-j);
                    else if(((x+i)<0)||((x+i)>=f.rows)) m[k] = f.at<uchar>(x-i,y+j);
                    else if(((y+i)<0)||((y+i)>=f.cols)) m[k] = f.at<uchar>(x+i,y-j);
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else m[k] = f.at<uchar>(x+i,y+j);
                    k++;
                }
            }
            int max = maxmin(m, 0);
            g.at<uchar>(x,y)=max;
        }
    }
    //exibe a imagem na janela
    imshow("Filtro da Maxima", g);
}

void PDI::filtroMinimo(){
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();

    //percorre a imagem inteira
    for(int x=0;x<f.rows;x++){
        for(int y=0;y<f.cols;y++){
            int m[9];
            int k=0;
            //percorre uma mascara 3x3
            for(int i=-1; i<2; i++){
                for(int j=-1; j<2; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) m[k] = f.at<uchar>(x-i,y-j);
                    else if(((x+i)<0)||((x+i)>=f.rows)) m[k] = f.at<uchar>(x-i,y+j);
                    else if(((y+i)<0)||((y+i)>=f.cols)) m[k] = f.at<uchar>(x+i,y-j);
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else m[k] = f.at<uchar>(x+i,y+j);
                    k++;
                }
            }
            int min = maxmin(m, 1);
            g.at<uchar>(x,y)=min;
        }
    }
    //exibe a imagem na janela
    imshow("Filtro da Minima", g);
}

float gaussiano(int x, int y){

    //sigma = 1

    float fator = (1/sqrt(2*M_PI));

    float resultado = fator*exp(-((pow(x,2))+(pow(y,2)))/2);

    return resultado;
}

void PDI::filtroGaussiano(){
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();

    //gerar matriz gaussiana

    float gaus[5][5];
    float count = 0;

    for(int i=-2; i<3; i++){
        for(int j=-2; j<3; j++){
            gaus[i+2][j+2] = gaussiano(i, j);
            count += gaus[i+2][j+2];
        }
    }

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            gaus[i][j] = gaus[i][j]/count;
        }
    }

    //percorre a imagem inteira
    for(int x=0;x<f.rows;x++){
        for(int y=0;y<f.cols;y++){
            float soma = 0;
            //percorre uma mascara 5x5
            for(int i=-2; i<3; i++){
                for(int j=-2; j<3; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) soma += f.at<uchar>(x-i,y-j)*gaus[i+2][j+2];
                    else if(((x+i)<0)||((x+i)>=f.rows)) soma += f.at<uchar>(x-i,y+j)*gaus[i+2][j+2];
                    else if(((y+i)<0)||((y+i)>=f.cols)) soma += f.at<uchar>(x+i,y-j)*gaus[i+2][j+2];
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else soma += f.at<uchar>(x+i,y+j)*gaus[i+2][j+2];
                }
            }
            g.at<uchar>(x,y)=soma;
        }
    }
    //exibe a imagem na janela
    imshow("Filtro da Gaussiana", g);

}

void PDI::filtroMedia(){
     Mat f = imagemCINZA.clone();
     Mat g = imagemCINZA.clone();
     //percorre a imagem inteira
     for(int x=0;x<f.rows;x++){
         for(int y=0;y<f.cols;y++){
             int soma = 0;
             //percorre uma mascara 3x3
             for(int i=-1; i<2; i++){
                 for(int j=-1; j<2; j++){
                     //caso pixels da mascara estejam fora dos limites da imagem
                     //usa-se espelhamento de pixels
                     if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) soma += f.at<uchar>(x-i,y-j);
                     else if(((x+i)<0)||((x+i)>=f.rows)) soma += f.at<uchar>(x-i,y+j);
                     else if(((y+i)<0)||((y+i)>=f.cols)) soma += f.at<uchar>(x+i,y-j);
                     //caso todos os pixels da mascara estejam dentro dos limites da imagem
                     else soma += f.at<uchar>(x+i,y+j);
                 }
             }
             g.at<uchar>(x,y)=soma/9;
         }
     }
     //exibe a imagem na janela
     imshow("Filtro da Media", g);
}

int medianaVetor(int m[]){

    //funcao para encontrar mediana de um vetor
    int aux;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if(m[i] < m[j]){
                aux = m[i];
                m[i] = m[j];
                m[j] = aux;
            }
        }
    }
    return m[4];
}

void PDI::filtroMediana(){
    Mat f = imagemCINZA.clone();
    Mat g = imagemCINZA.clone();

    for(int x=0; x<f.rows;x++){
        for(int y=0; y<f.cols;y++){
            int m[9];
            int k=0;
            for(int i=-1; i<2; i++){
                for (int j=-1; j<2; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=f.rows && (y+j)>=f.cols)) m[k] = f.at<uchar>(x-i,y-j);
                    else if(((x+i)<0)||((x+i)>=f.rows)) m[k] = f.at<uchar>(x-i,y+j);
                    else if(((y+i)<0)||((y+i)>=f.cols)) m[k] = f.at<uchar>(x+i,y-j);
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else m[k] = f.at<uchar>(x+i,y+j);
                    k++;
                }
            }
            int mediana = medianaVetor(m);
            g.at<uchar>(x,y)=mediana;
        }
    }
    imshow("Filtro da Mediana", g);
}

void PDI::brilhoHSV(){
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];
    //percorre apenas o plano V
    uchar brilho=0;
    int k=50; //constante para aumentar ou diminuir o brilho
    for(int x=0;x<imagemHSV.rows;x++)
    {
        for(int y=0;y<imagemHSV.cols;y++)
        {
            brilho = V.at<uchar>(x, y);
            if ((brilho+k) < 0) brilho = 0;
            else if ((brilho+k) > 255) brilho = 255;
            else brilho+=k;
            V.at<uchar>(x, y) = brilho;
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Ajuste de Brilho", imagemSaida);
}

void PDI::medianaHSV(){
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];
    //percorre apenas o plano V
    for(int x=0;x<imagemHSV.rows;x++){
        for(int y=0;y<imagemHSV.cols;y++){
            int m[9];
            int k=0;
            for(int i=-1; i<2; i++){
                for (int j=-1; j<2; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=V.rows && (y+j)>=V.cols)) m[k] = V.at<uchar>(x-i,y-j);
                    else if(((x+i)<0)||((x+i)>=V.rows)) m[k] = V.at<uchar>(x-i,y+j);
                    else if(((y+i)<0)||((y+i)>=V.cols)) m[k] = V.at<uchar>(x+i,y-j);
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else m[k] = V.at<uchar>(x+i,y+j);
                    k++;
                }
            }
            int mediana = medianaVetor(m);
            V.at<uchar>(x,y)=mediana;
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro Mediana", imagemSaida);
}

void PDI::equalizacaoHistogramaHSV(){

    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];

    //HISTOGRAMA - IMAGEM EM ESCALA DE CINZA
    float h[256];

    //inicia o vetor com zeros
    for(int i=0;i<256;i++) h[i]=0;

    //calcula a distribuicao dos niveis de cinza
    for(int x=0;x<V.rows;x++){
        for(int y=0;y<V.cols;y++){
            h[(int)V.at<uchar>(x,y)] += 1;
        }
    }

    //vetor do histograma normalizado
    float tamanho = V.rows * V.cols;
    for(int i=0;i<256;i++){
        h[i]=h[i]/tamanho;
    }

    //vetor do histograma normalizado acumulado
    for(int x=1; x<256; x++){
        h[x] = h[x]+h[x-1];
    }

    //calculo do histograma equalizado
    for(int x=0; x<256; x++){
        h[x] = round((255)*h[x]);
    }

    //imagem final
    for(int x=0;x<V.rows;x++){
        for(int y=0;y<V.cols;y++){
            V.at<uchar>(x,y) = (int)h[(int)V.at<uchar>(x,y)];
        }
    }

    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Imagem Equalizada", imagemSaida);

}

void PDI::gaussianaHSV(){
    Mat imagemHSV;
    //converte uma imagem RGB para HSV
    cvtColor(imagemRGB, imagemHSV, CV_BGR2HSV);
    vector<Mat> planosHSV;
    //divide a imagem HSV em 3 planos de pixels
    split(imagemHSV, planosHSV);
    //obtem apenas o plano V
    Mat V = planosHSV[2];

    float gaus[5][5];
    float count = 0;

    for(int i=-2; i<3; i++){
        for(int j=-2; j<3; j++){
            gaus[i+2][j+2] = gaussiano(i, j);
            count += gaus[i+2][j+2];
        }
    }

    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            gaus[i][j] = gaus[i][j]/count;
        }
    }

    //percorre apenas o plano V
    for(int x=0;x<imagemHSV.rows;x++){
        for(int y=0;y<imagemHSV.cols;y++){
            float soma=0;
            for(int i=-1; i<2; i++){
                for (int j=-1; j<2; j++){
                    //caso pixels da mascara estejam fora dos limites da imagem
                    //usa-se espelhamento de pixels
                    if(((x+i)<0 && (y+j)<0)||((x+i)>=V.rows && (y+j)>=V.cols)) soma += V.at<uchar>(x-i,y-j)*gaus[i+2][j+2];
                    else if(((x+i)<0)||((x+i)>=V.rows)) soma += V.at<uchar>(x-i,y+j)*gaus[i+2][j+2];
                    else if(((y+i)<0)||((y+i)>=V.cols)) soma += V.at<uchar>(x+i,y-j)*gaus[i+2][j+2];
                    //caso todos os pixels da mascara estejam dentro dos limites da imagem
                    else soma += V.at<uchar>(x+i,y+j)*gaus[i+2][j+2];
                }
            }
            V.at<uchar>(x,y)=soma;
        }
    }
    //combina os 3 planos de pixels (H,S,V) novamente
    merge(planosHSV,imagemHSV);
    Mat imagemSaida;
    //converte uma imagem HSV para RGB
    cvtColor(imagemHSV, imagemSaida, CV_HSV2BGR);
    //exibe a imagem RGB na janela
    imshow("Filtro Gaussiana", imagemSaida);
}
