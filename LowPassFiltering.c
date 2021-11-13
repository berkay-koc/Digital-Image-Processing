#include <stdio.h>
#include <string.h>

int **fileToMatrix(char fileName[20], char fileFormat[8], int* cols, int* rows, int filterSize); //pgm dosyasýný matrise çevirecek olan fonksiyon
void freeMatrice(int **img, int cols, int rows); //matrisleri free edecek fonksiyon
int **meanFilter(int **imageMatrix, int cols, int rows, int filterSize); //ortalama filtresi uygulayacak olan fonksiyon
void savePGM(int **imageMatrix, int cols, int rows, char filterType, char fileName[20]); //elde edilen output matrisi save edecek olan fonksiyon
int **medianFilter(int **imageMatrix, int cols, int rows, int filterSize); //medyan filtresi uygulayacak olan fonksiyon
void insertionSort(int arr[], int n); //ortalama filtresini sýralayacak fonksiyon

int main(){
	int rows = 0, cols = 0, i, j, filterSize; // satýr sütun sayýsý bilgileri, indisler ve filtre boyutu
	int **image, **meanFilteredMatrix, **medianFilteredMatrix; //resmin iþlenmesi için gerekli matrisler
	char fileFormat[8], fileName[20]; //seçilen file'a ulaþmak için gerekli char arrayleri
	printf("please enter file format(please enter binary for P5 file or text for P2 file): ");
	gets(fileFormat);
	if(strcmp(fileFormat, "binary") == 0 || strcmp(fileFormat, "text") == 0){ //doðru girdi olup olmadýðý kontrol ediliyor
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
	strcat(fileName, ".pgm"); //fileName oluþturuluyor.
	
	image = fileToMatrix(fileName, fileFormat, &cols, &rows, filterSize); //pgm dosyasýný matrise çevirecek fonksiyon gerekli argümanlarla çaðrýlýyor
	
	meanFilteredMatrix = (int**)malloc(sizeof(int*) * (rows)); //dinamik olarak ortalama filtresinin sonucu için matris oluþturuluyor.
    for(i = 0; i < (rows); i++) {
        meanFilteredMatrix[i] = (int*)malloc(sizeof(int) * (cols));
    }
    
    medianFilteredMatrix = (int**)malloc(sizeof(int*) * (rows)); // dinamik olarak medyan filtresinin sonucu için matris oluþturuluyor.
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
	int i, j, r, c, maxVal; //satýr sütun sayýsý bilgileri ve indisler
	FILE *image; //pgm dosyasýndaki image
	char filePath[30] = "images/"; //images dosyasýnýn içerisindeki pgm dosyalarýna eriþmek için gerekli olan pathi oluþturmak için
	int **imageMatrix; //pgm dosyasýndaki matrisin aktarýlacaðý matris
	strcat(filePath, fileName);
	printf("%s\n", filePath); 
    
	if(strcmp(fileFormat, "binary") == 0){ //binary file için P5 deðeri okunup satýr sütun bilgileri alýnýyor
		image = fopen(filePath, "rb");
		fscanf(image, "P5\n%d %d\n", cols, rows);
	}
	else{								   //text file için P2 deðeri okunup satýr sütun bilgileri alýnýyor
		image = fopen(filePath, "r+");
		fscanf(image, "P2\n%d %d\n", cols, rows);
	}
	while(getc(image) != '\n'); // comment satýrlarý (varsa) 70-71-72. satýrlar sayesinde okunuyor
	while (getc(image) == '#'){
    while (getc(image) != '\n');}
	fseek(image, -1, SEEK_CUR);
	
    maxVal = fscanf(image, "%d\n", &maxVal); // pgm dosyasýnýn 3. satýrýndaki maxValue deðeri okunuyor
	
    imageMatrix = (int**)malloc(sizeof(int*) * (*rows)); //pgm dosyasýndaki bilgilerin aktarýlacaðý matris dinamik olarak oluþturuluyor
    for(i = 0; i < (*rows); i++) {
        imageMatrix[i] = (int*)malloc(sizeof(int) * (*cols));
    }
    
    if(strcmp(fileFormat, "binary") == 0){ // binary dosyasý ise karakterler integer hallerinde alýnýyor.
    	for(i=0; i<(*rows); i++){
			for(j=0; j<(*cols); j++){
				unsigned char c = getc(image);
				imageMatrix[i][j] = c;
			}
		}
	}	
	else{									// text dosyasý ise sayýlar direkt alýnýyor.
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
	int i, j, k, l, sum; //indisler ve ortalamayý bulmak için gerekli sum deðiþkeni
	int **outputMatrix; 
	int roamIndex; //filtre büyüklüðüne göre matriste gezinmek için gerekli olan deðiþken. filtre büyüklüðünün yarýsýdýr.
	int meanMat[filterSize][filterSize]; //ortlama matrisi. filterSize*filterSize büyüklüðündedir.
	
    outputMatrix = (int**)malloc(sizeof(int*) * rows); //output matrisi dinamik olarak oluþturuluyor.
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
    
	for(i=0; i<filterSize; i++){ // ortalama matrisi oluþturuluyor.
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
					sum += meanMat[k][l] * imageMatrix[i-(roamIndex)+k][j-(roamIndex)+l];//filterSize*filterSize kadar bir kare içindeki sayýlar toplanýyor
				}
			}
			outputMatrix[i][j] = sum/(filterSize*filterSize);//toplam sayý matrisin büyüklüðüne bölünerek ortalama deðer elde ediliyor.
		}
	}
	return outputMatrix;
}

int **medianFilter(int **imageMatrix, int cols, int rows, int filterSize){
	int i, j, k, l, sum; //indisler ve ortalamayý bulmak için gerekli sum deðiþkeni
	int medianFilterArray[filterSize*filterSize];
	int **outputMatrix;
	int roamIndex; //filtre büyüklüðüne göre matriste gezinmek için gerekli olan deðiþken. filtre büyüklüðünün yarýsýdýr.
	
    outputMatrix = (int**)malloc(sizeof(int*) * rows);
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
    
	roamIndex = filterSize/2;
	
	for(i=roamIndex; i<rows-roamIndex; i++){
		for(j=roamIndex; j<cols-roamIndex; j++){
			for(k=0; k < filterSize; k++){
				for(l=0; l < filterSize; l++){
					medianFilterArray[k*filterSize+l] = imageMatrix[i-(roamIndex)+k][j-(roamIndex)+l]; //medyan filtresi için gerekli deðerler bir diziye atýýyor
				}
			}
			sortArray(medianFilterArray, filterSize*filterSize);//medyan deðerinin bulunmasý için dizi sýralanýyor
			outputMatrix[i][j] = medianFilterArray[(filterSize*filterSize)/2]; //medyan deðeri matrisin gözüne yazýlýyor
		}
	}
	return outputMatrix;
}

void sortArray(int arr[], int n){ //median sort için sýralama fonksiyonu. Insertion sort kullanýlýyor
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
	
	if(filterType == 0){ //eðer dosya ortlama filtresi ile oluþturulduysa dosya isminin sonuna mean ekleniyor. 
		strcpy(str, "mean");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	else{				//eðer dosya medyan filtresi ile oluþturulduysa dosya isminin sonuna median ekleniyor. 
		strcpy(str, "median");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	
	output = fopen(str, "w+");
	
	fputs("P2\n", output);//kaydedilecek dosya açýlarak P2 formatý için gerekli bilgiler yazýlýyor.
	fprintf(output, "%d %d\n", cols, rows); //sütun ve satýr sayýsý ekleniyor.
	fputs("255\n", output);//resim grayscale olduðundan 255 yazýlýyor.
	
	for(i=0; i< rows; i++){ //matristeki deðerler tek tek ekleniyor.
		for(j=0; j< cols; j++){
			fprintf(output, "%d ", imageMatrix[i][j]);
		}
	}
	fclose(output);
}

void freeMatrice(int **img, int cols, int rows){//matris için dinamik olan yerler kapatýlýyor.
	int i;
	for(i = 0; i < rows; i++)
    	free(img[i]);
	free(img);
	printf("MATRIX FREED!\n"); //Our beloved matrix is now free...
}
