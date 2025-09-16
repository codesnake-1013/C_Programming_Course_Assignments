#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct{
	char name[11];
	long long number;
	char sex;
	char major[51];
	int class;
	int record;
}STU;

int menu();
int readList();
void count();
void cur_situation();
void pre_situation();
void inq_by_number();
void inq_by_name();
 
int main(){
	
	STU stu[100];
	int sum=0;
	int cstu[100];
	int cnt=0;
	int cur;
	int situa[120]={0};
		
	while(1){
		switch(menu()){
			case 1:
				sum=readList(stu);
				break;
			case 2:
				if(sum > 0) {
				    count(sum, stu,&cnt,cstu,&cur,situa);
				} 
				else {
				    printf("请先读入学生信息！\n\n");
				}
				break;
			case 3:
				if(sum>0){
					cur_situation(stu,cstu,cur);
				}
				else{
					printf("请先读入学生信息！\n\n");
				}
				break;
			case 4:
				if(sum>0){
					pre_situation(stu,cnt,sum);
				}
				else{
					printf("请先读入学生信息！\n\n");
				}				
				break;
			case 5:
				if(sum>0){
					inq_by_number(stu,sum,situa);
				}
				else{
					printf("请先读入学生信息！\n\n");
				}
				break;
			case 6:
				if(sum>0){
					inq_by_name(stu,sum,situa);
				}
				else{
					printf("请先读入学生信息！\n\n");
				}				
				break;
			case 0:
				printf("退出成功！\n");
				exit(0);
			default:
				printf("请输入正确的数字！\n\n");
				break;
		}
	}
	return 0;
}

int menu(){
	int select;
	printf("==========随机点名系统==========\n");
	printf("1.读入学生名单\n");
	printf("2.随机点名\n");
	printf("3.本次点名出勤情况\n");
	printf("4.以往点名出勤情况\n");
	printf("5.通过学号查询出勤情况\n");
	printf("6.通过姓名查询出勤情况\n");
	printf("0.退出程序\n");
	printf("================================\n\n");
	printf("请输入选项：");
	scanf("%d",&select);
	printf("\n");
	return select;
}

int readList(STU stu[]){
	FILE *fp;
	fp=fopen("stus.txt","r");
	if(fp == NULL) {  
	    printf("无法打开文件！\n");
	    return 0;
	}
	int i=0;
	int sum=-1;
	
	while(i<120&&!feof(fp)){
		fscanf(fp, "%s %lld %c %s %d %d", stu[i].name,&(stu[i].number),&(stu[i].sex),stu[i].major,&(stu[i].class),&(stu[i].record));
		sum++;
		i++;
	}
	
	fclose(fp);
	printf("\n读入了%d个学生的信息。\n\n\n",sum);
	return sum;
}

void count(int sum, STU *stu,int *cnt,int *cstu,int *cur,int *situa) {
    char con;
    
    int used[100] = {0};  
    *cur=0;
    int turn=0;
	do {
        turn++;
		for(int k = 0; k < 5; k++) {
            srand(time(NULL) + k);  
            int r;
            do {
                r = rand() % sum;
            } while(used[r]);  
            
            used[r] = 1;
            
            printf("\n随机点名:\n");
            printf("姓名：%s\t    学号：%lld      性别：%c\n",stu[r].name, stu[r].number, stu[r].sex);                   
            printf("专业：%s      班级：%d\n",stu[r].major, stu[r].class);                   
            printf("出勤扣分：%d（满分0分）\n\n",stu[r].record);                                      
            printf("此人出勤了吗？（y/n）：");
            getchar();  
            char yn = getchar();
            
            if(yn == 'y' || yn == 'Y') {
                printf("恭喜%s同学本次出勤考察合格\n", stu[r].name);
                situa[r]=1;
				if(stu[r].record<=-6){
					printf("但是您的出勤扣分已经超过最高限，课程考勤不达标，将无权参加期末考试\n\n");
				}
            } 
            else if(yn == 'n' || yn == 'N') {
                stu[r].record--;
                printf("\n很遗憾您没有来上课，将被扣除1分\n");
                situa[r]=-1;
                if(stu[r].sex == 'M') {
                    stu[r].record--;
                    printf("由于您是男生，还将被多扣1分\n\n");
                }
                if(stu[r].record<=-6){
					printf("您的出勤扣分已经超过最高限，课程考勤不达标，将无权参加期末考试\n\n");
				}
				cstu[(*cur)++]=r;
                
				(*cnt)++;             
            }
        }
        
        printf("\n是否继续点名？（y/n）：");
        getchar();  
        con = getchar();
        
    } while(con == 'y' || con == 'Y');
    FILE *fp = fopen("stus.txt", "w");
	if(fp != NULL) {
	    for(int i = 0; i < sum; i++) {
	        fprintf(fp, "%s %lld %c %s %d %d\n",stu[i].name, stu[i].number, stu[i].sex,stu[i].major, stu[i].class, stu[i].record);
	    }
		fclose(fp);
	}
	printf("是否需要出勤分析？（y/n）");
	getchar();
	if(getchar()=='y'){
		printf("\n\n总点名人数%d\n出勤人数%d\n出勤人数百分比%.2f%%\n\n",5*turn,5*turn-*cur,(5.0*turn-*cur)/(5*turn)*100);
		if(*cur>0){
			printf("以下同学未出勤，请班长与同学联系\n");
		
			for(int i=0;i<*cur;i++){
				printf("姓名：%s    学号：%lld      性别：%c\n",stu[cstu[i]].name, stu[cstu[i]].number, stu[cstu[i]].sex);                   
				printf("专业：%s      班级：%d\n",stu[cstu[i]].major, stu[cstu[i]].class);                   
				printf("出勤扣分：%d（满分0分）\n\n",stu[cstu[i]].record);
			}
		}
		else{
			printf("点到的同学全部出勤\n\n");
		}
	}    
	          
}


void cur_situation(STU stu[],int cstu[],int cur){
	if(cur>0){
		for(int i=0;i<cur;i++){
			printf("姓名：%s    学号：%lld      性别：%c\n",stu[cstu[i]].name, stu[cstu[i]].number, stu[cstu[i]].sex);                   
			printf("专业：%s      班级：%d\n",stu[cstu[i]].major, stu[cstu[i]].class);                   
			printf("出勤扣分：%d（满分0分）\n\n",stu[cstu[i]].record);
		}
	}
	else{
		printf("无\n\n");
	}
}

void pre_situation(STU stu[],int cnt,int sum){
	
		int q=0;
		int a[100];
		int k=0;
		for(int i=0;i<sum;i++){
			if(stu[i].record<0){
				a[k++]=i;
				q++;				
			}
		}
		if(q>0){
			for (int i = 0; i < k - 1; i++) {
		    	for (int j = 0; j < k - i - 1; j++) {
			    	if (stu[a[j]].record > stu[a[j + 1]].record) {
		                    
		    	    int temp = a[j];
			        a[j] = a[j + 1];
			        a[j + 1] = temp;
			        }
			    }
			}
			printf("共计%d名学生有缺勤情况\n",cnt);
			for(int i=0;i<q;i++){
				printf("姓名：%s\t    学号：%lld    出勤扣分：%d（满分0分）\n",stu[a[i]].name,stu[a[i]].number,stu[a[i]].record);
			}
	    }
	    else{
			printf("无未出勤的学生\n\n");
		}
	
}

void inq_by_number(STU stu[],int sum,int situa[]){
	long long re_num;
	printf("请输入学号：");
	scanf("%lld",&re_num);
	int flag=1;
	for(int i=0;i<sum;i++){
		if(re_num==stu[i].number){
			flag=0;
			printf("您的姓名为：%s\n到目前为止，您的出勤扣分情况：已扣%d\n",stu[i].name,stu[i].record);
			if(situa[i]==1){
				printf("今天的点名你出勤了\n\n");				
			}
			else if(situa[i]==0){
				printf("今天未点到你的名字\n\n");
			}
			else{
				printf("今天的点名你未到\n\n");
			}
			break;
		}									
	}
	if(flag){
		printf("查无此人\n\n");
	}
}

void inq_by_name(STU stu[], int sum, int situa[]) {
    char re_name[21];
    printf("请输入姓名：");
    scanf("%20s", re_name);  // 限制输入长度，防止缓冲区溢出
    int flag = 1;

    for (int i = 0; i < sum; i++) {
        if (strcmp(re_name, stu[i].name) == 0) {  // 使用strcmp比较字符串
            flag = 0;
            printf("您的姓名为：%s\n到目前为止，您的出勤扣分情况：已扣%d\n", stu[i].name, stu[i].record);
            if (situa[i] == 1) {
                printf("今天的点名你出勤了\n\n");
            } else if (situa[i] == 0) {
                printf("今天未点到你的名字\n\n");
            } else {
                printf("今天的点名你未到\n\n");
            }
            break;
        }
    }

    if (flag) {
        printf("查无此人\n\n");
    }
}