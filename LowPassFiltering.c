#include <stdio.h>
#include <string.h>

int **fileToMatrix(char fileName[20], char fileFormat[3], int* cols, int* rows);
void freeMatrice(int **img, int cols, int rows);
int **meanFilter(int **imageMatrix, int cols, int rows);
void savePGM(int **imageMatrix, int cols, int rows, char filterType, char fileName[20]);
int **medianFilter(int **imageMatrix, int cols, int rows);
void insertionSort(int arr[], int n);
void printMatrix(int **matrix, int cols, int rows);

int main(){
	int rows = 0, cols = 0, i, j;
	int **image;
	char fileFormat[8]; //valid ones are "binary" or "text"
	printf("please enter file format(binary or text): ");
	gets(fileFormat);
	if(strcmp(fileFormat, "binary") == 0 || strcmp(fileFormat, "text") == 0){
		printf("this format is valid\n");
	}
	else{
		printf("please enter a valid format\n");
		return 0;
	}
	char fileName[20];
	printf("please enter file name: ");
	gets(fileName);
	strcat(fileName, ".pgm");
	fileToMatrix(fileName, fileFormat, &cols, &rows);
	
	return 0;
}

int **fileToMatrix(char fileName[20], char fileFormat[8], int *cols, int *rows){
	int i, j, maxVal;
	FILE *image;
	char filePath[30] = "images/";
	strcat(filePath, fileName);
	printf("%s\n", filePath); //images/lenaN.pgm
	
	int **meanFilteredMatrix;
	int **medianFilteredMatrix;
    meanFilteredMatrix = (int**)malloc(sizeof(int*) * (*rows));
    for(i = 0; i < (*rows); i++) {
        meanFilteredMatrix[i] = (int*)malloc(sizeof(int) * (*cols));
    }
    
    medianFilteredMatrix = (int**)malloc(sizeof(int*) * (*rows));
    for(i = 0; i < (*rows); i++) {
        medianFilteredMatrix[i] = (int*)malloc(sizeof(int) * (*cols));
    }
	
	//get rows, cols and max value
	if(strcmp(fileFormat, "binary") == 0){
		image = fopen(filePath, "rb");
		fscanf(image, "P5\n%d %d\n", cols, rows);
	}
	else{
		image = fopen(filePath, "r+");
		fscanf(image, "P2\n%d %d\n", cols, rows);
	}
    maxVal = fscanf(image, "%d\n", &maxVal);
	
	int **imageMatrix;
    imageMatrix = (int**)malloc(sizeof(int*) * (*rows));
    for(i = 0; i < (*rows); i++) {
        imageMatrix[i] = (int*)malloc(sizeof(int) * (*cols));
    }
	int c = (*cols);
	int r = (*rows);
    if(strcmp(fileFormat, "binary") == 0){
    	for(i=0; i<r; i++){
			for(j=0; j<c; j++){
				unsigned char c = getc(image);
				imageMatrix[i][j] = c;
			}
		}
	}
	else{
		for(i=0; i<r; i++){
			for(j=0; j<c; j++){
				fscanf(image, "%d", &imageMatrix[i][j]);
			}
		}
	}
	
	//printMatrix(imageMatrix, c, r);
	printf("stop0");
	meanFilteredMatrix = meanFilter(imageMatrix, c, r);
	printf("stop1");
	savePGM(meanFilteredMatrix, c, r, 0, fileName);
	printf("stop2");
	medianFilteredMatrix = medianFilter(imageMatrix, c, r);
	printf("stop3");
	savePGM(medianFilteredMatrix, c, r, 1, fileName);
	printf("stop4");
	
	freeMatrice(medianFilteredMatrix, c, r);
	freeMatrice(meanFilteredMatrix, c, r);
	freeMatrice(imageMatrix, c, r);
	
	fclose(image);
	return imageMatrix;
	
}

int **meanFilter(int **imageMatrix, int cols, int rows){
	int i, j, k, l, sum;
	int **outputMatrix;
    outputMatrix = (int**)malloc(sizeof(int*) * rows);
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
    printf("stop11");
	int meanMat[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
	
	for(i=1; i<rows-1; i++){
		for(j=1; j<cols-1; j++){
			sum = 0;
			for(k=0; k<3; k++){
				for(l=0; l<3; l++){
					sum += meanMat[k][l] * imageMatrix[i-1+k][j-1+l];
				}
			}
			outputMatrix[i][j] = sum/(9);
		}
	}
	return outputMatrix;
}

int **medianFilter(int **imageMatrix, int cols, int rows){
	int i, j, k, l, sum;
	int medianFilterArray[3*3];
	int **outputMatrix;
	
    outputMatrix = (int**)malloc(sizeof(int*) * rows);
    for(i = 0; i < rows; i++) {
        outputMatrix[i] = (int*)malloc(sizeof(int) * cols);
    }
	
	for(i=1; i<rows-1; i++){
		for(j=1; j<cols-1; j++){
			for(k=0; k<3; k++){
				for(l=0; l<3; l++){
					medianFilterArray[k*3+l] = imageMatrix[i-1+k][j-1+l];
				}
			}
			sortArray(medianFilterArray, 9);
			outputMatrix[i][j] = medianFilterArray[9/2];
		}
	}
	return outputMatrix;
}

void sortArray(int arr[], int n){
    int i, j, key;
    for (i = 1; i < n; i++){
        key = arr[i];
        j = i - 1;
		
        while (j >= 0 && arr[j] > key){
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// file type 0 = binary 
// file type 1 = text

//filter type 0 = mean
//filter type 1 = median

void savePGM(int **imageMatrix, int cols, int rows, char filterType, char fileName[20]){ //filename = lena.pgm
	char str[40];
	int i, j;
	if(filterType == 0){
		strcpy(str, "mean");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	else{
		strcpy(str, "median");
		strcat(str, fileName);
		printf("\n%s\n", str);
	}
	
	FILE *output = fopen(str, "w+");
	
	fputs("P2\n", output);
	fprintf(output, "%d %d\n", cols, rows);
	printf("%d %d", cols, rows);
	fputs("255\n", output);
	
	for(i=0; i< rows; i++){
		for(j=0; j< cols; j++){
			fprintf(output, "%d ", imageMatrix[i][j]);
		}
	}
	fclose(output);
}

void freeMatrice(int **img, int cols, int rows){
	int i;
	for(i = 0; i < rows; i++)
    	free(img[i]);
	free(img);
	printf("FREED!\n");
}

void printMatrix(int **matrix, int cols, int rows){
	int i, j;
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("%4d", matrix[i][j]);
		}
		printf("\n");
	}
}











