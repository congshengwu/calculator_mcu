#include "calculate.h"

#define LEN 99//����ʽ��󳤶�,�����Զ���


double calculate(char *c){
	//c�Ǵ����ʽ�ַ�����ָ��
	double result=0;//���շ��ر���
	int c_len=strlen(c);
	int i,k;//i���������±�
	double temp,j;
	double opnd[LEN];//������ջ,�ܴ�10��������,����С����
	int opnd_index=0;
	char optr[LEN]="";//������ջ
	
	int optr_index=0;
	/*�� ) ����ջ��ʱ�򣬻Ὣ (����)��ļ���ʽ��������,
	��ջ��һ����������ϵ�����,��ջָ��ָ��(,
	����������ջ������ +-����* �Ͳ����Ȳ���Ҫ������,ֱ���滻 ) */
	int CalByButton_r=0;//����0�� ) ����ʽ��֮ǰ
	char num_stack[LEN]="";//��ʱ��ջ
	int num_stack_index=0;//��ʱ���±�
	double num_temp=0;//��ʱ��
	optr[0]=0;
	//ע�⣺while�����һ�������� ���ֵģ���������������ж϶�����ִ��
	while(c_len) {//+-����.�ϲ������ַ�����ͷ (�����ڿ�ͷ����Ҫ����
		c_len--;
		//*c���������������С����
		if((*c<='9' && *c>='0') || (*c=='.')) {
			num_stack[num_stack_index]=*c;
			num_stack_index++;
			c++;
			continue;
		}
		//*c�� + ���
		if(*c=='+') {
			num_stack_index--;
			if(num_stack_index==-1){// ) �Ѿ���ȡһ�������ˣ�) �������� �Ͳ�������������
				num_stack_index=0;
				goto c1;
			}
			//��+ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
			for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
				if(num_stack[i] != '.')
					num_temp+=((num_stack[i]-'0')*j);
				
				if(num_stack[i]=='.'){
					for(k=num_stack_index-i;k>0;k--) {
						num_temp/=10;
					}
					j=0.1;//��һ��j����1
				}
			}
			opnd[opnd_index]=num_temp;
			opnd_index++;
			num_temp=0;//��ʱ����ʼ��
			num_stack_index=0;//��ʱջ�±��ʼ��
			c1:
			//�� + ��һЩ����
			if(optr[optr_index]==0) {//���optrΪ��,ֱ�ӽ��룬֮��ֱ�������ռ���֮ǰ������Ϊ����
				optr[optr_index]='+';//�Ȳ�++����Ϊ�������ж���һ����ջ����
			}//���optr��Ϊ�գ�+ �� ֱ�ӳ������Ľ������㣬�ٴ�
			else if(optr[optr_index]=='+') {//���������=���������������opnd,+�Ų���ϵ�
				opnd_index-=2;//��ȥ��ӵ� �� ����һ֮��λ��Ϊ�����Ǹ�
				temp=opnd[opnd_index]+opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;//
				
				optr[optr_index]='+';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='-') {
				opnd_index-=2;
				temp=opnd[opnd_index]-opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
				optr[optr_index]='+';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='*') {//+����
				opnd_index-=2;
				temp=opnd[opnd_index]*opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				while(1){//���� 6+8-4*2+6-10 �� bug
					if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){
							case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
						}
						opnd_index++;
					}
					break;
				}
				optr[optr_index]='+';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='/') {
				opnd_index-=2;
				temp=opnd[opnd_index]/opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
				while(1){//���� 6+8-4/2+6-10 �� bug
					if (optr_index - 1 < 0)//��ֹ����Խ��
                        break;
					if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){
							case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
						}
						opnd_index++;
					}
					break;
				}
				optr[optr_index]='+';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='('){//����� ( ֱ�Ӵ��ȥ
				if(CalByButton_r==1){//������������ �� ) ����� �� ( ; �������� ) ǰ�� (
					if((optr_index-1)>=0){//���� 6*(1-2*2)+3��=0 ������ ��֮ǰӦ�����ж�һ�� ԭ ( �µķ���*/											
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){ //���������ջ�����ţ���ʱ ) û���ȥ
							case '+':opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-':opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
							case '*':opnd[opnd_index]=opnd[opnd_index]*opnd[opnd_index+1];break;
							case '/':opnd[opnd_index]=opnd[opnd_index]/opnd[opnd_index+1];break;
							//�������case�Ǵ��� ( �»��� ( ����� 
							case '(':optr_index++;opnd_index++;break;//ֻ�轫optr_index��opnd_index��λ����							
						}
						opnd_index++;
						optr[optr_index]='+';
						CalByButton_r=0;//��λ
						while(1){
							if (optr_index - 1 < 0)//��ֹ����Խ��
								break;
							if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
								optr_index--;
								opnd_index-=2;
								switch(optr[optr_index]){
									case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
									case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
								}
							opnd_index++;
							}
							optr[optr_index]='+';
							break;
						}
						c++;
						continue;
					}
					optr[optr_index]='+';
					CalByButton_r=0;//��λ
					c++;
					continue;
				}
				optr_index++;
				optr[optr_index]='+';
			}
			c++;
			continue;
		}
		//*c�� - ������� + һ��  ���Ŵ����� ����(��
		if(*c=='-') {
			num_stack_index--;
			if(num_stack_index==-1){// ) �Ѿ���ȡһ�������ˣ�) �������� �Ͳ�������������
				num_stack_index=0;
				goto c2;
			}
			//��-ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
			for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
				if(num_stack[i] != '.')
					num_temp+=((num_stack[i]-'0')*j);
				
				if(num_stack[i]=='.'){
					for(k=num_stack_index-i;k>0;k--) {
						num_temp/=10;
					}
					j=0.1;//��һ��j����1
				}
			}
			opnd[opnd_index]=num_temp;
			opnd_index++;
			num_temp=0;//��ʱ����ʼ��
			num_stack_index=0;//��ʱջ�±��ʼ��
			c2:
			//�� - ��һЩ����
			if(optr[optr_index]==0) {//���optrΪ��,ֱ�ӽ���
				optr[optr_index]='-';//�Ȳ�++����Ϊ�������ж���һ����ջ����
			}//���optr��Ϊ�գ�+ �� ֱ�ӳ������Ľ������㣬�ٴ�
			else if(optr[optr_index]=='+') {//���������=���������������opnd,+�Ų���ϵ�
				opnd_index-=2;//��ȥ��ӵ� �� ����һ֮��λ��Ϊ�����Ǹ�
				temp=opnd[opnd_index]+opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;//
				
				optr[optr_index]='-';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='-') {
				opnd_index-=2;
				temp=opnd[opnd_index]-opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
				optr[optr_index]='-';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='*') {//+����
				opnd_index-=2;
				temp=opnd[opnd_index]*opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				while(1){//���� 6+8-4/2+6-10 �� bug
					if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){
							case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
						}
						opnd_index++;
					}
					break;
				}
				optr[optr_index]='-';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='/') {
				opnd_index-=2;
				temp=opnd[opnd_index]/opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				while(1){//���� 6+8-4/2+6-10 �� bug
					if (optr_index - 1 < 0)//��ֹ����Խ��
                        break;
					if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){
							case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
						}
						opnd_index++;
					}
					break;
				}
				optr[optr_index]='-';//ֱ���滻��ԭ���±�Ϊ����ķ���
			}else if(optr[optr_index]=='('){//����� ( ֱ�Ӵ��ȥ
				if(CalByButton_r==1){//�������� �� ) ����� �� ( ����
					if((optr_index-1)>=0){//���� 6*(1-2*2)+3��=0 ������ /*��֮ǰӦ�����ж�һ�� ԭ ( �µķ���*/	
						optr_index--;
						opnd_index-=2;
						switch(optr[optr_index]){ //���������ջ�����ţ���ʱ ) û���ȥ
							case '+':opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
							case '-':opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
							case '*':opnd[opnd_index]=opnd[opnd_index]*opnd[opnd_index+1];break;
							case '/':opnd[opnd_index]=opnd[opnd_index]/opnd[opnd_index+1];break;
							case '(':optr_index++;opnd_index++;break;
							//����� ( �»��� ( ֻ�轫optr_index��opnd_index��λ����
						}
						opnd_index++;
						optr[optr_index]='-';
						CalByButton_r=0;//��λ
						c++;
						continue;
					}
					optr[optr_index]='-';
					CalByButton_r=0;//��λ
					while(1){
						if (optr_index - 1 < 0)//��ֹ����Խ��
							break;
						if(optr[optr_index-1]=='+' || optr[optr_index-1]=='-'){
							optr_index--;
							opnd_index-=2;
							switch(optr[optr_index]){
								case '+': opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
								case '-': opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
							}
							opnd_index++;
						}
						optr[optr_index]='-';
						break;
					}
					c++;
					continue;
				}
				optr_index++;
				optr[optr_index]='-';
			}
			c++;
			continue;
		}
		//*c�� �� ���
		if(*c=='*') {
			num_stack_index--;
			if(num_stack_index==-1){// ) �Ѿ���ȡһ�������ˣ�) �������� �Ͳ�������������
				num_stack_index=0;
				goto c3;
			}
			//��*ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
			for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
				if(num_stack[i] != '.')
					num_temp+=((num_stack[i]-'0')*j);
				
				if(num_stack[i]=='.'){
					for(k=num_stack_index-i;k>0;k--) {
						num_temp/=10;
					}
					j=0.1;//��һ��j����1
				}
			}
			opnd[opnd_index]=num_temp;
			opnd_index++;
			num_temp=0;//��ʱ����ʼ��
			num_stack_index=0;//��ʱջ�±��ʼ��
			//�� * ��һЩ����
			c3:
			if(optr[optr_index]==0) {//���optrΪ��,ֱ�ӽ���
				optr[optr_index]='*';//�Ȳ�++����Ϊ�������ж���һ����ջ����
			}//���optr��Ϊ��
			else if(optr[optr_index]=='+') {//����+�Ų�����ֱ�Ӵ��ȥ
				optr_index++;
				optr[optr_index]='*';
			}else if(optr[optr_index]=='-') {
				optr_index++;
				optr[optr_index]='*';
			}else if(optr[optr_index]=='*') {//*����*,������=��������������,*����ԭ��*λ��,Ҳ���ǲ��ò���
				opnd_index-=2;
				temp=opnd[opnd_index]*opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
			}else if(optr[optr_index]=='/') {
				opnd_index-=2;
				temp=opnd[opnd_index]/opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
				optr[optr_index]='*';//*�滻/��λ��
			}else if(optr[optr_index]=='('){//����� ( ֱ�Ӵ��ȥ
				if(CalByButton_r==1){//�������� �� ) ����� �� (
					if((optr_index-1)>=0){//���� 6*(1-2*2)+3��=0 ������ /*��֮ǰӦ�����ж�һ�� ԭ ( �µķ���*/	
						if(optr[optr_index-1]!='+' && optr[optr_index-1]!='-'){//����6-(5+2)*6 *��֮ǰ �� - �Ÿ�����
							optr_index--;
							opnd_index-=2;
							switch(optr[optr_index]){ //���������ջ�����ţ���ʱ ) û���ȥ
								case '+':opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
								case '-':opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
								case '*':opnd[opnd_index]=opnd[opnd_index]*opnd[opnd_index+1];break;
								case '/':opnd[opnd_index]=opnd[opnd_index]/opnd[opnd_index+1];break;
								case '(':optr_index++;opnd_index++;break;
							//����� ( �»��� ( ֻ�轫optr_index��opnd_index��λ����
							}
							opnd_index++;
							optr[optr_index]='*';
							CalByButton_r=0;//��λ
							c++;
							continue;
						}
					}
					optr[optr_index]='*';
					CalByButton_r=0;//��λ
					c++;
					continue;
				}
				optr_index++;
				optr[optr_index]='*';
			}
			c++;
			continue;
		}
		//*c�� / ���
		if(*c=='/') {
			num_stack_index--;
			if(num_stack_index==-1){// ) �Ѿ���ȡһ�������ˣ�) �������� �Ͳ�������������
				num_stack_index=0;
				goto c4;
			}
			//��*ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
			for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
				if(num_stack[i] != '.')
					num_temp+=((num_stack[i]-'0')*j);
				
				if(num_stack[i]=='.'){
					for(k=num_stack_index-i;k>0;k--) {
						num_temp/=10;
					}
					j=0.1;//��һ��j����1
				}
			}
			opnd[opnd_index]=num_temp;
			opnd_index++;
			num_temp=0;//��ʱ����ʼ��
			num_stack_index=0;//��ʱջ�±��ʼ��
			c4:
			//�� / ��һЩ����
			if(optr[optr_index]==0) {//���optrΪ��,ֱ�ӽ���
				optr[optr_index]='/';//�Ȳ�++����Ϊ�������ж���һ����ջ����
			}//���optr��Ϊ��
			else if(optr[optr_index]=='+') {//����+�Ų�����ֱ�Ӵ��ȥ
				optr_index++;
				optr[optr_index]='/';
			}else if(optr[optr_index]=='-') {
				optr_index++;
				optr[optr_index]='/';
			}else if(optr[optr_index]=='*') {
				opnd_index-=2;
				temp=opnd[opnd_index]*opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
				
				optr[optr_index]='/';// /�滻*��λ��
			}else if(optr[optr_index]=='/') {// /����/,������=��������������,/����ԭ��/λ��,Ҳ���ǲ��ò���
				opnd_index-=2;
				temp=opnd[opnd_index]/opnd[opnd_index+1];
				opnd[opnd_index]=temp;
				opnd_index++;
			}else if(optr[optr_index]=='('){//����� ( ֱ�Ӵ��ȥ
				if(CalByButton_r==1){//�������� �� ) ����� �� (					
					if((optr_index-1)>=0){//���� 6*(1-2*2)+3��=0 ������ /*��֮ǰӦ�����ж�һ�� ԭ ( �µķ���*/	
						if(optr[optr_index-1]!='+' && optr[optr_index-1]!='-'){//����6-(5+2)/6 /��֮ǰ �� - �Ÿ�����
							optr_index--;
							opnd_index-=2;
							switch(optr[optr_index]){ //���������ջ�����ţ���ʱ ) û���ȥ
								case '+':opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
								case '-':opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
								case '*':opnd[opnd_index]=opnd[opnd_index]*opnd[opnd_index+1];break;
								case '/':opnd[opnd_index]=opnd[opnd_index]/opnd[opnd_index+1];break;
								case '(':optr_index++;opnd_index++;break;
							//����� ( �»��� ( ֻ�轫optr_index��opnd_index��λ����
							}
							opnd_index++;
							optr[optr_index]='/';
							CalByButton_r=0;//��λ
							c++;
							continue;
						}
					}
					optr[optr_index]='/';
					CalByButton_r=0;//��λ
					c++;
					continue;
				}
				optr_index++;
				optr[optr_index]='/';
			}
			c++;
			continue;
		}
		//�� ( ���
		if(*c=='('){//ֱ���������ջ
			if(optr_index==0){
				if(optr[0]!='+' && optr[0]!='-' && optr[0]!='*' && optr[0]!='/' && optr[0]!='('){
					optr[optr_index]='(';
					c++;
					continue;
				}
			}
			optr_index++;
			optr[optr_index]='(';
			c++;
			continue;
		}
		//�� ) ���
		if(*c==')'){
			num_stack_index--;
			//��*ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
			for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
				if(num_stack[i] != '.')
					num_temp+=((num_stack[i]-'0')*j);
				
				if(num_stack[i]=='.'){
					for(k=num_stack_index-i;k>0;k--) {
						num_temp/=10;
					}
					j=0.1;//��һ��j����1
				}
			}
			opnd[opnd_index]=num_temp;
			opnd_index++;
			num_temp=0;//��ʱ����ʼ��
			num_stack_index=0;//��ʱջ�±��ʼ��
			
			while(1){//��while�Ǵ��� ������� (1-5*2) Ҳ���������ڵĶ���ʽ
				opnd_index-=2;
				switch(optr[optr_index]){ //���������ջ�����ţ���ʱ ) û���ȥ
					case '+':opnd[opnd_index]=opnd[opnd_index]+opnd[opnd_index+1];break;
					case '-':opnd[opnd_index]=opnd[opnd_index]-opnd[opnd_index+1];break;
					case '*':opnd[opnd_index]=opnd[opnd_index]*opnd[opnd_index+1];break;
					case '/':opnd[opnd_index]=opnd[opnd_index]/opnd[opnd_index+1];break;
				}
				opnd_index++;
				optr_index--;//�����ջ�½�һλ
				if(optr[optr_index]=='('){
					CalByButton_r=1; //������˵��
					break;
				}
				
			}
		
		}
		
		c++;//ָ�������ʱ���ƶ�
	}
	
	///////////////////////////////////////////
		num_stack_index--;
		//��+ǰһ�����������opnd��num_temp�Ǵ��ַ����гɹ���ȡ��һ����,�����Ѿ����double
		for(i=num_stack_index,j=1; i>=0; i--,j*=10) {
			if(num_stack[i] != '.')
				num_temp+=((num_stack[i]-'0')*j);
			
			if(num_stack[i]=='.'){
				for(k=num_stack_index-i;k>0;k--) {
					num_temp/=10;
				}
				j=0.1;//��һ��j����1
			}
		}
		opnd[opnd_index]=num_temp;
		opnd_index++;
		num_temp=0;//��ʱ����ʼ��
		num_stack_index=0;//��ʱջ�±��ʼ��
	
	i=optr_index;
	do{
		switch(optr[i]){
			case '+':opnd[i]=opnd[i]+opnd[i+1];break;
			case '-':opnd[i]=opnd[i]-opnd[i+1];break;
			case '*':opnd[i]=opnd[i]*opnd[i+1];break;
			case '/':opnd[i]=opnd[i]/opnd[i+1];break;
		}
		i--;
	}while(i!=-1);
	result=opnd[0];
	
	return result;
}