package main

type Account struct {balance float64; password string;}

func main(){

	var acct1 Account;
	acct1.balance = 0.0;
	acct1.password = "foo";
	deposit("foo", 100.00, acct1);
}

func deposit(pass string, amount float64, acct Account){
	if acct.password == pass {
		acct.balance += amount;
		print("New balance:");
		print(acct.balance);
	}else{
		print("Wrong password");
	}
}

func withdraw(pass string, amount float64, acct Account){
	if acct.password == pass {
		if acct.balance >= amount {
			acct.balance -= amount;
			print("New balance:");
			print(acct.balance);
		}else {
			print("Not enough funds");
		}
		
	}else {
		print("Wrong password");
	}
}