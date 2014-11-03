#include <stdio.h>
#include <stdio.h>

typedef unsigned char BYTE;
 
int addFile(int newFileStartCluster, char *fileInName, FILE *fileOut) {
    static int totalFiles = 0;
    int neededCluster, newFileStartFatBytes, startWriteRootOffset;
	long start, fileSize, startDataAreaOffset;
    int i, j;
    static BYTE byte0x1200[0x1200] = {0};
    static BYTE byte0x1c00[0x1c00] = {0};
    char tempPath[11] = "           ";
    char *dot;
    FILE *fileIN = fopen(fileInName, "rb");
    if(fileIN == NULL) {
        printf("Error, need %s!\n", fileInName);
        exit(1);
    }

    //����KERNEL.SYS��Ҫ��clust����
    fseek(fileIN, 0L, SEEK_END);
    fileSize = ftell(fileIN);
    neededCluster = (int) (fileSize/512 + ((fileSize%512==0)?0:1));
    newFileStartFatBytes = newFileStartCluster*3/2;

    //printf("neededCluster %d\n", neededCluster);

    if(newFileStartCluster % 2 == 0) {
        int clusterLen = neededCluster+((neededCluster%2)?1:0);
        //int tmpArray[clusterLen];
        int *tmpArray = (int *)malloc(clusterLen*sizeof(int));
        for(i = 0; i < neededCluster-1; i++) {
            tmpArray[i] = newFileStartCluster+i+1;
        }
        if(neededCluster%2==0) {
            tmpArray[neededCluster-1] = 0x0fff;
        }
        else {
            tmpArray[neededCluster-1] = 0x0fff;
            tmpArray[neededCluster] = 0x0;
        }
        start = newFileStartFatBytes;
        if(start%3!=0)
            start = (newFileStartFatBytes/2)*2;

        for(i=0,j=0; j < clusterLen; i+=3,j+=2) {
          byte0x1200[start+i] = tmpArray[j];
          byte0x1200[start+i+1] = ((tmpArray[j+1]&0xf)<<4) + ((tmpArray[j]&0xf00)>>8);
          byte0x1200[start+i+2] = (tmpArray[j+1]&0xff0)>>4;
        }
        free(tmpArray);
    } else {  //newFileStartCluster is odd
        int clusterLen = neededCluster+((neededCluster%2)?1:2);
        //int tmpArray[clusterLen];
        int *tmpArray = (int *)malloc(clusterLen*sizeof(int));
        tmpArray[0] = 0x0fff;
        for(i = 1; i < neededCluster; i++) 
            tmpArray[i] = newFileStartCluster+i;

        if(neededCluster%2==0) {
            tmpArray[neededCluster] = 0x0fff;
            tmpArray[neededCluster+1] = 0x0;
        } else
            tmpArray[neededCluster] = 0x0fff;

        start = newFileStartFatBytes-1;
        for(i=0,j=0; j < clusterLen; i+=3,j+=2) {
            byte0x1200[start+i] =  tmpArray[j];
            byte0x1200[start+i+1] =  ((tmpArray[j+1]&0xf)<<4)+((tmpArray[j]&0xf00)>>8);
            byte0x1200[start+i+2] =  (tmpArray[j+1]&0xff0)>>4;
        }
        free(tmpArray);
    }

    //�޸�Root Directory
    startWriteRootOffset = (totalFiles++)<<5;
    dot = strrchr(fileInName, '.' );
    strncpy(tempPath, fileInName, dot-fileInName);
    strcpy(tempPath+8, dot+1);

    strcpy(byte0x1c00+startWriteRootOffset, tempPath);

    byte0x1c00[startWriteRootOffset+0xB] = 0x20;                           //FileAttr;
    byte0x1c00[startWriteRootOffset+0x1A] = (newFileStartCluster);         //start cluster
    byte0x1c00[startWriteRootOffset+0x1B] = (newFileStartCluster>>8); 
    byte0x1c00[startWriteRootOffset+0x1C] = (fileSize&0xff);               //file size
    byte0x1c00[startWriteRootOffset+0x1D] = (fileSize>>8);
    byte0x1c00[startWriteRootOffset+0x1E] = (fileSize>>16);
    byte0x1c00[startWriteRootOffset+0x1F] = (fileSize>>24);

    fseek(fileOut, 0x200, SEEK_SET);
    //д�� FAT1
    for(i=0; i<0x1200; i++)
        fputc(byte0x1200[i], fileOut);
    //д�� FAT2
    for(i=0; i<0x1200; i++)
        fputc(byte0x1200[i], fileOut);
    //д�� Root directory
    for(i=0; i<0x1c00; i++)
        fputc(byte0x1c00[i], fileOut);
    //д�� fileIn
    startDataAreaOffset = ((33+(long)newFileStartCluster-2)<<9)-0x200-0x1200-0x1200-0x1c00;
    fseek(fileOut, startDataAreaOffset, SEEK_CUR);
    rewind(fileIN);
    while (( i = fgetc( fileIN )) != EOF)
        fputc(i, fileOut);

    fclose(fileIN);
    return newFileStartCluster+neededCluster;
}


int main(int argc, char* argv[]) {
    long i;
    int c, newFileStartCluster, nextCluster;
    FILE *fileBoot = fopen("boot.bin", "rb");
    FILE *fileOutImage = fopen("FunnyOS.img", "wb");
    if(fileBoot == NULL) {
        printf("Error, need boot.bin!\n");
        return 1;
    }
     
    //����imgΪ1.44M
	printf("##### create 1.44M FunnyOS.img ####\n");
    for(i = 0; i < 0x168000; i++)
        fputc(0, fileOutImage);
    rewind(fileOutImage);

    //��boot.bin����dostest.img
	printf("##### copy boot.bin to first 512 byte of FunnyOS.img ####\n");
    while (( c = fgetc( fileBoot )) != EOF)
        fputc(c, fileOutImage);
    fclose(fileBoot);	

	printf("##### add KERNEL.SYS ####\n");
    nextCluster = addFile(2, "KERNEL.SYS", fileOutImage);
	//printf("##### add COMMAND.COM ####\n");
    //addFile(nextCluster, "COMMAND.COM", fileOutImage);

    fclose(fileOutImage);	
	printf("##### Success! Use FunnyOS.img to boot ####\n");
    return 0;
}