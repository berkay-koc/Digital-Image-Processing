#include <stdio.h>
#include <string.h>

int **fileToMatrix(char fileName[20], char fileFormat[8], int* cols, int* rows, int filterSize); //pgm dosyas�n� matrise �evirecek olan fonksiyon
void freeMatrice(int **img, int cols, int rows); //matrisleri free edecek fonksiyon
int **meanFilter(int **imageMatrix, int cols, int rows, int filterSize); //ortalama filtresi uygulayacak olan fonksiyon
void savePGM(int **imageMatrix, int cols, int rows, char filterType, char fileName[20]); //elde edilen output matrisi save edecek olan fonksiyon
int **medianFilter(int **imageMatrix, int cols, int rows, int filterSize); //medyan filtresi uygulayacak olan fonksiyon
void insertionSort(int arr[], int n); //ortalama filtresini s�ralayacak fonksiyon

int main(){
	int rows = 0, cols = 0, i, j, filterSize; // sat�r s�tun say�s� bilgileri, indisler ve filtre boyutu
	int **image, **meanFilteredMatrix, **medianFilteredMatrix; //resmin i�lenmesi i�in gerekli matrisler
	char fileFormat[8], fileName[20]; //se�ilen file'a ula�mak i�in gerekli char arrayleri
	printf("please enter file format(please enter binary for P5 file or text for P2 file): ");
	gets(fileFormat);
	if(strcmp(fileFormat, "binary") == 0 || strcmp(fileFormat, "text") == 0){ //do�ru girdi olup olmad��� kontrol ediliyor
		printf("this format is valid\n");
	}
	else{
		printf("please enter a valid format\n");
		return 0;
	}
	printf("please enter file name: ");
	gets(fileName);
	printf("please enter filter size: ");
	scanf("%d", &filterSize);
	strcat(fileName, ".pgm"); //fileName olu�turuluyor.
	
	image = fileToMatrix(fileName, fileFormat, &cols, &rows, filterSize); //pgm dosyas�n� matrise �evirecek fonksiyon gerekli arg�manlarla �a�r�l�yor
	
	meanFilteredMatrix = (int**)malloc(sizeof(int*) * (rows)); //dinamik olarak ortalama filtresinin sonucu i�in matris olu�turuluyor.
    for(i = 0; i < (rows); i++) {
        meanFilteredMatrix[i] = (int*)malloc(sizeof(int) * (cols));
    }
    
    medianFilteredMatrix = (int**)malloc(sizeof(int*) * (rows)); // dinamik olarak medyan filtresinin sonucu i�in matris olu�turuluyor.
    for(i = 0; i < (rows); i++) {
        medianFilteredMatrix[i] = (int*)malloc(sizeof(int) * (cols));
    }
	
	meanFilteredMatrix = meanFilter(image, cols, rows, filterSize);
	savePGM(meanFilteredMatrix, cols, rows, 0, fileName);
	medianFilteredMatrix = medianFilter(image, cols, rows, filterSize);
	savePGM(medianFilteredMatrix, cols, rows, 1, fileName);
	
	freeMatrice(medianFilteredMatrix, cols, rows);
	freeMatrice(meanFilteredMatrix, cols, rows);
	freeMatrice(image, cols, rows);
	
	return 0;
}

int **fileToMatrix(char fileName[20], char fileFormat[8], int *cols, int *rows, int filterSize){
	int i, j, r, c, maxVal; //sat�r s�tun say�s� bilgileri ve indisler
	FILE *image; //pgm dosyas�ndaki image
	char filePath[30] = "images/"; //images dosyas�n�n i�erisindeki pgm dosyalar�na eri�mek i�in gerekli olan pathi olu�turmak i�in
	int **imageMatrix; //pgm dosyas�ndaki matrisin aktar�laca�� matris
	strcat(filePath, fileName);
	printf("%s\n", filePath); 
    
	if(strcmp(fileFormat, "binary") == 0){ //binary file i�in P5 de�eri okunup sat�r s�tun bilgileri al�n�yor
		image = fopen(filePath, "rb");
		fscanf(image, "P5\n%d %d\n", cols, rows);
	}
	else{								   //text file i�in P2 de�eri okunup sat�r s�tun bilgileri al�n�yor
		image = fopen(filePath, "r+");
		fscanf(image, "P2\n%d %d\n", cols, rows);
	}
	while(getc(image) != '\n'); // comment sat�rlar� (varsa) 70-71-72. sat�rlar sayesinde okunuyor
	while (getc(image) == '#'){
    while (getc(image) != '\n');}
	fseek(image, -1, SEEK_CUR);
	
    maxVal = fscanf(image, "%d\n", &maxVal); // pgm dosyas�n�n 3. sat�r�ndaki maxValue de�eri okunuyor
	
    imageMatrix = (int**)malloc(sizeof(int*) * (*rows)); //pgm dosyas�ndaki bilgilerin aktar�laca�� matris dinamik olarak olu�turuluyor
    for(i = 0; i < (*rows); i++) {
        imageMatrix[i] = (int*)malloc(sizeof(int) * (*cols));
    }
    
    if(strcmp(fileFormat, "binary") == 0){ // binary dosyas� ise karakterler integer hallerinde al�n�yor.
    	for(i=0; i<(*rows); i++){
			for(j=0; j<(*cols); j++){
				unsigned char c = getc(image);
				imageMatrix[i][j] = c;
			}
		}
	}	
	else{									// text dosyas� ise say�lar direkt al�n�yor.
		for(i=0; i<(*rows); i++){
			for(j=0; j<(*cols); j++){
				fscanf(image, "%d", &imageMatrix[i][j]);
			}
		}
	}
	
	fclose(image);
	return imageMatrix;
}

int **meanFilter(int **imageMatrix, int cols, int rows, int filterSize){ 
	int i, j, k, l, sum; //indisler ve ortalamay� bulmak i�in gerekli sum de�i�keni
	int **outputMatrix; 
	int roamIndex; //filtre b�y�kl���ne g�re matriste gezinmek i�in gerekli olan de�i�ken. filtre b�y�kl���n�n yar�s�d�r.
	int meanMat[filterSize][filterSize]; //ortlama matrisi. filterSize*filterSize b�y�kl���ndedir.
	
    outputMatrix = (int**)malloc(sizeof(int*) * rows); //output matrisi dinamik olarak olu�turuluyor.
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
    
	for(i=0; i<filterSize; i++){ // ortalama matrisi olu�turuluyor.
		for(j=0; j<filterSize; j++){
			meanMat[i][j] = 1;
		}
	}
	
	roamIndex = filterSize/2;
	
	for(i=roamIndex; i<rows-roamIndex; i++){
		for(j=roamIndex; j<cols-roamIndex; j++){
			sum = 0;
			for(k=0; k < filterSize; k++){
				for(l=0; l < filterSize; l++){
					sum += meanMat[k][l] * imageMatrix[i-(roamIndex)+k][j-(roamIndex)+l];//filterSize*filterSize kadar bir kare i�indeki say�lar toplan�yor
				}
			}
			outputMatrix[i][j] = sum/(filterSize*filterSize);//toplam say� matrisin b�y�kl���ne b�l�nerek ortalama de�er elde ediliyor.
		}
	}
	return outputMatrix;
}

int **medianFilter(int **imageMatrix, int cols, int rows, int filterSize){
	int i, j, k, l, sum; //indisler ve ortalamay� bulmak i�in gerekli sum de�i�keni
	int medianFilterArray[filterSize*filterSize];
	int **outputMatrix;
	int roamIndex; //filtre b�y�kl���ne g�re matriste gezinmek i�in gerekli olan de�i�ken. filtre b�y�kl���n�n yar�s�d�r.
	
    outputMatrix = (int**)malloc(sizeof(int*) * rows);
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
    
	roamIndex = filterSize/2;
	
	for(i=roamIndex; i<rows-roamIndex; i++){
		for(j=roamIndex; j<cols-roamIndex; j++){
			for(k=0; k < filterSize; k++){
				for(l=0; l < filterSize; l++){
					medianFilterArray[k*filterSize+l] = imageMatrix[i-(roamIndex)+k][j-(roamIndex)+l]; //medyan filtresi i�in gerekli de�erler bir diziye at��yor
				}
			}
			sortArray(medianFilterArray, filterSize*filterSize);//medyan de�erinin bulunmas� i�in dizi s�ralan�yor
			outputMatrix[i][j] = medianFilterArray[(filterSize*filterSize)/2]; //medyan de�eri matrisin g�z�ne yaz�l�yor
		}
	}
	return outputMatrix;
}

void sortArray(int arr[], int n){ //median sort i�in s�ralama fonksiyonu. Insertion sort kullan�l�yor
    int i, j, temp;
    for (i = 1; i < n; i++){
        temp = arr[i];
        j = i - 1;
		
        while (j >= 0 && arr[j] > temp){
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = temp;
    }
}

// file type 0 = binary 
// file type 1 = text

//filter type 0 = mean
//filter type 1 = median

void savePGM(int **imageMatrix, int cols, int rows, char filterType, char fileName[20]){ 
	char str[40];
	int i, j;
	FILE *output;
	
	if(filterType == 0){ //e�er dosya ortlama filtresi ile olu�turulduysa dosya isminin sonuna mean ekleniyor. 
		strcpy(str, "mean");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	else{				//e�er dosya medyan filtresi ile olu�turulduysa dosya isminin sonuna median ekleniyor. 
		strcpy(str, "median");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	
	output = fopen(str, "w+");
	
	fputs("P2\n", output);//kaydedilecek dosya a��larak P2 format� i�in gerekli bilgiler yaz�l�yor.
	fprintf(output, "%d %d\n", cols, rows); //s�tun ve sat�r say�s� ekleniyor.
	fputs("255\n", output);//resim grayscale oldu�undan 255 yaz�l�yor.
	
	for(i=0; i< rows; i++){ //matristeki de�erler tek tek ekleniyor.
		for(j=0; j< cols; j++){
			fprintf(output, "%d ", imageMatrix[i][j]);
		}
	}
	fclose(output);
}

void freeMatrice(int **img, int cols, int rows){//matris i�in dinamik olan yerler kapat�l�yor.
	int i;
	for(i = 0; i < rows; i++)
    	free(img[i]);
	free(img);
	printf("MATRIX FREED!\n"); //Our beloved matrix is now free...
}
