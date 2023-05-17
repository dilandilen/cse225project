//15019912 dilan dilen 150119911 dilara nur mentes
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
	//struct for customer
typedef struct customer
{
	char name[20];
	char surname[30];
	int customerid;
	char customertype[20];
	struct customer * nextcust;
	double totaldebt;
	struct loan * loanptr;
}

customer;
//struct for loans
typedef struct loan
{
	char loanid[30];
	char type[30];
	float totalamount;
	int totalinstallmentnum;
	char processdate[11];
	struct loan * nextloan;
	struct installment * insptr;
}

loan;
//struct for installments
typedef struct installment
{
	char insid[30];
	short ispaid;
	char installmentdate[11];
	float amount;
	struct installment * nextins;
}

installment;
//head of customers loans and installment list
customer *customer_list = NULL;

void readCustomers()
{
	//open file
	FILE *fp = fopen("customer.txt", "r");
	if (fp == NULL)
	{
		printf("Error: Could not open file 'customers.txt'\n");
		return;
	}

	//holding the last customer
	customer *current_customer = NULL;

	char name[20], surname[30], type[50];
	//read data from file
	while (fscanf(fp, "%s %s %s\n", name, surname, type) == 3)
	{
		customer *new_cust = (customer*) malloc(sizeof(customer));
		//if new_cust is first customer id is 1 otherwise a++
		new_cust->customerid = current_customer ? current_customer->customerid + 1 : 1;
		//The data we get from the file is assigned to the customer data
		strcpy(new_cust->name, name);
		strcpy(new_cust->surname, surname);
		strcpy(new_cust->customertype, type);
		// Initially all customers' loanptr is null totaldebt 0
		new_cust->loanptr = NULL;
		new_cust->totaldebt = 0;
		new_cust->nextcust = NULL;
		// if the first customer
		if (customer_list == NULL)
		{
			customer_list = new_cust;
			current_customer = new_cust;
		}
		else
		{
			current_customer->nextcust = new_cust;
			current_customer = new_cust;
		}
	}

	//close customer.txt
	fclose(fp);
}

void printCustomers()
{
	// open file for writing
	FILE *fout = fopen("output.txt", "w");
	if (fout == NULL)
	{
		printf("Error: Could not create file 'output.txt'\n");
		return;
	}

	printf("------------------------------------------------------------\n");
	fprintf(fout, "------------------------------------------------------------\n");

	//temp is holding the head of customers.
	customer *temp = customer_list;
	// print temps variables both output.txt and console while end of the customers
	while (temp != NULL)
	{
		printf("%d - ", temp->customerid);
		fprintf(fout, "%d - ", temp->customerid);

		printf("%s ", temp->name);
		fprintf(fout, "%s ", temp->name);

		printf("%s ", temp->surname);
		fprintf(fout, "%s ", temp->surname);

		printf(" type : %s - ", temp->customertype);
		fprintf(fout, " type : %s - ", temp->customertype);

		printf("total debt : %d \n", temp->totaldebt);
		fprintf(fout, "total debt : %d \n", temp->totaldebt);

		printf("-----------------------------------------------------------\n");
		fprintf(fout, "-----------------------------------------------------------\n");

		temp = temp->nextcust;
	}

	// close file
	fclose(fout);
}

//update loans id
void updateLoansID()
{
	customer *current_cust = customer_list;

	while (current_cust != NULL)
	{
		int loan_count = 0;
		loan *current_loan = current_cust->loanptr;

		while (current_loan != NULL)
		{
			loan_count++;
			sprintf(current_loan->loanid, "%dL%d", current_cust->customerid, loan_count);
			current_loan = current_loan->nextloan;
		}

		current_cust = current_cust->nextcust;
	}
}

//to print right format of date
char *convertDate(const char *input)
{
	int year, month, day;
	char *output = (char*) malloc(11* sizeof(char));
	sscanf(input, "%d/%d/%d", &year, &month, &day);
	sprintf(output, "%02d/%02d/%04d", day, month, year);
	return output;
}

void readLoans()
{
	FILE *fp = fopen("loans.txt", "r");
	if (fp == NULL)
	{
		printf("Error: Could not open file 'loans.txt'\n");
		return;
	}

	char name[20], surname[30], type[50], processdate[11];
	float amount;
	int installmentnum;
	loan *curr_loan = NULL;
	//read line by line divide each line to 6 variablesa
	while (fscanf(fp, "%s %s %s %f %d %s\n", name, surname, type, &amount, &installmentnum, processdate) == 6)
	{
		customer *current_cust = customer_list;
		while (current_cust != NULL)
		{
			//if name and surname in loan.txt are equal to customer
			if (strcmp(current_cust->name, name) == 0 && strcmp(current_cust->surname, surname) == 0)
			{
			 	//create new loan
				loan *new_loan = (loan*) malloc(sizeof(loan));
				strcpy(new_loan->type, type);
				new_loan->totalamount = amount;
				new_loan->totalinstallmentnum = installmentnum;
				//parse processs date
                char *token = strtok(processdate, "/");
                int day, month, year;
                sscanf(token, "%d", &day);
                token = strtok(NULL, "/");
                sscanf(token, "%d", &month);
                token = strtok(NULL, "/");
                sscanf(token, "%d", &year);
                sprintf(new_loan->processdate, "%d/%02d/%02d", year, month, day);


				new_loan->insptr = NULL;
				new_loan->nextloan = NULL;
				// if first loan of current customer
				if (current_cust->loanptr == NULL)
				{
					current_cust->loanptr = new_loan;
					curr_loan = new_loan;
				}	//otherwise sort list ascending order
				else
				{
					loan *prev_loan = NULL;
					loan *curr_loan = current_cust->loanptr;

					while (curr_loan != NULL && strcmp(curr_loan->processdate, new_loan->processdate) < 0)
					{
						prev_loan = curr_loan;
						curr_loan = curr_loan->nextloan;
					}

					if (prev_loan == NULL)
					{
						new_loan->nextloan = current_cust->loanptr;
						current_cust->loanptr = new_loan;
					}
					else if (curr_loan == NULL)
					{
						prev_loan->nextloan = new_loan;
					}
					else
					{
						new_loan->nextloan = curr_loan;
						prev_loan->nextloan = new_loan;
					}
				}

				break;
			}

			current_cust = current_cust->nextcust;
		}

		//assign ýd
		updateLoansID();
	}

	fclose(fp);
}

void printLoans()
{
	// open file for writing
	FILE *fout = fopen("output.txt", "a");
	if (fout == NULL)
	{
		printf("Error: Could not create file 'output.txt'\n");
		return;
	}

	//print variables both console
	printf("------------------------------------------------------------\n");
	fprintf(fout, "------------------------------------------------------------\n");

	customer *current_cust = customer_list;

	while (current_cust != NULL)
	{
		printf("%d - %s %s - type : %s - total debt: %d\n", current_cust->customerid, current_cust->name, current_cust->surname, current_cust->customertype, current_cust->totaldebt);
		fprintf(fout, "%d - %s %s - type : %s - total debt: %d\n", current_cust->customerid, current_cust->name, current_cust->surname, current_cust->customertype, current_cust->totaldebt);

		loan *current_loan = current_cust->loanptr;
		while (current_loan != NULL)
		{
			printf("- %s:", current_loan->loanid);
			fprintf(fout, "- %s:", current_loan->loanid);

			printf("   %s -", current_loan->type);
			fprintf(fout, "   %s -", current_loan->type);

			printf("   %.2f -", current_loan->totalamount);
			fprintf(fout, "   %.2f -", current_loan->totalamount);
			//formatted date
			char *x = convertDate(current_loan->processdate);
			printf("   %s -", x);
			fprintf(fout, "   %s -", x);

			printf("   %d \n", current_loan->totalinstallmentnum);
			fprintf(fout, "   %d \n", current_loan->totalinstallmentnum);

			current_loan = current_loan->nextloan;
		}

		printf("------------------------------------------------------------\n");
		fprintf(fout, "------------------------------------------------------------\n");

		current_cust = current_cust->nextcust;
	}

	// close file
	fclose(fout);
}

void printInstallments()
{
	// open file for writing
	FILE *fout = fopen("output.txt", "a");
	if (fout == NULL)
	{
		printf("Error: Could not create file 'output.txt'\n");
		return;
	}

	printf("------------------------------------------------------------\n");
	fprintf(fout, "------------------------------------------------------------\n");

	customer *current_cust = customer_list;
	while (current_cust != NULL)
	{
		loan *current_loan = current_cust->loanptr;
		printf("%d - %s %s - type : %s - total debt: %d\n", current_cust->customerid, current_cust->name, current_cust->surname, current_cust->customertype, current_cust->totaldebt);
		fprintf(fout, "%d - %s %s - type : %s - total debt: %d\n", current_cust->customerid, current_cust->name, current_cust->surname, current_cust->customertype, current_cust->totaldebt);

		while (current_loan != NULL)
		{
			printf("     %s:", current_loan->loanid);
			fprintf(fout, "     %s:", current_loan->loanid);

			printf("   %s -", current_loan->type);
			fprintf(fout, "   %s -", current_loan->type);

			printf("   %.2f -", current_loan->totalamount);
			fprintf(fout, "   %.2f -", current_loan->totalamount);

			char *x = convertDate(current_loan->processdate);
			printf("   %s -", x);
			fprintf(fout, "   %s -", x);

			printf("   %d \n", current_loan->totalinstallmentnum);
			fprintf(fout, "   %d \n", current_loan->totalinstallmentnum);

			installment *current_ins = current_loan->insptr;
			while (current_ins != NULL)
			{
				char *x = convertDate(current_ins->installmentdate);

				printf("       %s -> %s- %.2f:", current_ins->insid, x, current_ins->amount);
				fprintf(fout, "       %s -> %s- %.2f:", current_ins->insid, x, current_ins->amount);

				if (current_ins->ispaid == 0)
				{
					printf(" To be Paid\n");
					fprintf(fout, " To be Paid\n");
				}
				else if (current_ins->ispaid == 1)
				{
					printf(" Paid\n");
					fprintf(fout, " Paid\n");
				}
				else
				{
					printf(" Delayed Payment\n");
					fprintf(fout, " Delayed Payment\n");
				}

				current_ins = current_ins->nextins;
			}

			current_loan = current_loan->nextloan;
		}

		current_cust = current_cust->nextcust;
	}

	fclose(fout);
}

void createInstallments()
{
	customer *current_cust = customer_list;
	while (current_cust != NULL)
	{
		loan *current_loan = current_cust->loanptr;
		while (current_loan != NULL)
		{
			//amount=total/num
			float amount = current_loan->totalamount / current_loan->totalinstallmentnum;
			int ins_count = 0;
			installment *current_ins = NULL;
			installment *prev_ins = NULL;
			installment *first_installment = NULL;
			// parse processtade
			int year, month, day;
			sscanf(current_loan->processdate, "%d/%d/%d", &year, &month, &day);

			for (int i = 1; i <= current_loan->totalinstallmentnum; ++i)
			{
				installment *new_installment = (installment*) malloc(sizeof(installment));
				sprintf(new_installment->installmentdate, "%d/%02d/%02d", year, month, day);
				new_installment->nextins = NULL;
				new_installment->ispaid = 0;
				new_installment->amount = amount;

				if (i == 1)
				{
					first_installment = new_installment;
					current_loan->insptr = first_installment;
				}
				else
				{
				 		//sort installment
					while (current_ins != NULL && strcmp(current_ins->installmentdate, new_installment->installmentdate) < 0)
					{
						prev_ins = current_ins;
						current_ins = current_ins->nextins;
					}

					prev_ins->nextins = new_installment;
				}

				prev_ins = new_installment;
				sprintf(new_installment->insid, "%sI%d", current_loan->loanid, i);
				month++;
				if (month > 12)
				{
					month = 1;
					year++;
				}
			}

			current_loan = current_loan->nextloan;
		}

		current_cust = current_cust->nextcust;
	}
}

void readPayments()
{
	// Open the payments.txt file
	FILE *fp = fopen("payments.txt", "r");
	if (fp == NULL)
	{
		printf("Error: Could not open file 'payments.txt'\n");
		return;
	}

	char loanid[30], payment[10];
	while (fscanf(fp, "%s %s", loanid, payment) == 2)
	{
		customer *current_cust = customer_list;
		while (current_cust != NULL)
		{
			loan *current_loan = current_cust->loanptr;

			while (current_loan != NULL)
			{
				if (strcmp(current_loan->loanid, loanid) == 0)
				{
					installment *current_ins = current_loan->insptr;
					if (strcmp(payment, "ALL") == 0)
					{
						while (current_ins != NULL)
						{
							current_ins->ispaid = 1;
							current_ins = current_ins->nextins;
						}
					}
					else
					{
						int installment_num = atoi(payment);
						int count = 1;
						while (current_ins != NULL)
						{
							if (count == installment_num)
							{
								current_ins->ispaid = 1;
								break;
							}

							count++;
							current_ins = current_ins->nextins;
						}
					}

					break;
				}

				current_loan = current_loan->nextloan;
			}

			current_cust = current_cust->nextcust;
		}
	}

	// Close the file
	fclose(fp);
}

int main()
{
	customer * customers;
	int option = 1000;
	while (option != 0)
	{
		printf("\n\n#############################################################\n");
		printf("1. read customers.\n");
		printf("2. print customers.\n");
		printf("3. read loans.\n");
		printf("4. print loans.\n");
		printf("5. create installments.\n");
		printf("6. print installments.\n");
		printf("7. read payments.\n");
		printf("8. find unpaid installments.\n");
		printf("9. delete completely paid installments.\n");
		printf("please select your option : \n");
		scanf("%d", &option);
		printf("#############################################################\n");

		switch (option)
		{
			case 1:
				readCustomers();
				break;
			case 2:
				printCustomers();
				break;
			case 3:
				readLoans();
				break;
			case 4:
				printLoans();
				break;
			case 5:
				createInstallments();
				break;
			case 6:
				printInstallments();
			case 7:
				readPayments();
				break;
			case 8:
				//findUnpaidInstallments function call here
				break;
			case 9:
				//DeletePaidInstallments function call here
				break;
			case 0:
				break;
			default:
				printf("invalid option\n");
		}
	}

	return 0;
}
