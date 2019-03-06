#include<iostream>
#include<string>
#include<vector>

// STUDENT ID: 16086937

using namespace std;

enum state { Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart };
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };

enum Options {
	Base_Plain = 1, Base_Spicy = 2, Base_Chocolate = 4, Base_Coconut = 8, Base_Fruity = 16,
	Filling_Choco = 32, Filling_Banana = 64, Filling_Strawberry = 128,
	Filling_Raspberry = 256, Filling_Apple = 512, Filling_Blackberry = 1024,
	Filling_Maple = 2048, Filling_Marshmallow = 4096, Filling_Cheese = 8192,
	Filling_Cheese_And_Ham = 16384, Filling_Caramel = 32768, Filling_Vanilla = 65536
}; // enum for the options so option code doesn't have to be manually input in the code, also easier to add in new option codes for future developers

class StateContext;

class State
{
protected:
	StateContext* CurrentContext;

public:

	State(StateContext* Context)
	{
		CurrentContext = Context;
	}
	virtual ~State(void) {}
	virtual void transition(void) {}
};

class StateContext // push different states into vectors?
{
protected:
	State* CurrentState = nullptr; // the current state is set to null pointer until the current state gets set to something else
	int stateIndex = 0;
	vector<State*> availableStates; // vector of pointers for the available states which will be push back the classes such as OutofPoptart
	vector<int> stateParameters; // this is for the state parameters such as the cost and poptart stock

public:
	virtual ~StateContext(void);
	virtual void setState(state newState);
	virtual int getStateIndex(void);
	virtual void setStateParam(stateParameter SP, int value);
	virtual int getStateParam(stateParameter SP);
};

StateContext::~StateContext(void)
{
	for (int i = 0; i < this->availableStates.size(); i++) delete this->availableStates[i]; // for loop to remove the states in the vector
	this->availableStates.clear();
	this->stateParameters.clear();
}

void StateContext::setState(state newState)
{
	this->CurrentState = availableStates[newState];
	this->stateIndex = newState;
	this->CurrentState->transition(); // points the current state to the transition
}

int StateContext::getStateIndex(void)
{
	return this->stateIndex;
}

void StateContext::setStateParam(stateParameter SP, int value)
{
	this->stateParameters[SP] = value; // allows the enum to be used along with the value to set the value of the state parameter
}

int StateContext::getStateParam(stateParameter SP)
{
	return this->stateParameters[SP]; // returns the state parameter enum value, so it can be used for calculations
}

class Transition // class for the transitions between the state, this is an abstract class, so the methods can be polymorphed so money can be inserted in the relevant states
{
public:
	virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
	virtual bool makeSelection(int) { cout << "Error!" << endl; return false; }
	virtual bool moneyRejected(void) { cout << "Error!" << endl; return false; }
	virtual bool addPoptart(int) { cout << "Error!" << endl; return false; }
	virtual bool dispense(void) { cout << "Error!" << endl; return false; }
};

class PoptartState : public State, public Transition // this class inherits from the interfaces to enable polymorphism and the states don't directly use the classes, so more states can be added if wished to
{
public:
	PoptartState(StateContext* Context) : State(Context) {}
};

///////////////////// Product class for poptart flavours \\\\\\\\\\\\\\\\\\\\\

class Product // this is the interface for the decorator pattern, so classes can have an inheritance/compositional relationship, allowing the product description to be morphed into the specific bases and fillings
{
	friend class Filling;
protected:
	string product_description;
	int itemCost = 0;
	
public:
	virtual void consume(void);
	virtual int cost(void);
	virtual string description(void);
};

void Product::consume(void) { 

	cout << "Consuming " + this->product_description << endl;
	delete this; // deletes the product to prevent memory leak
}

int Product::cost(void) {

	return this->itemCost; // returns the item cost, so any added classes can use the variable and set the cost to how they want it
}

string Product::description(void) {

	return this->product_description; // returns the description, so any added classes can use the variable to polymorph the item name
}

///////////////////// DECORATOR PATTERN \\\\\\\\\\\\\\\\\\\\\
//////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

class Poptart : public Product
{
public:
	Poptart(void)
	{
		this->product_description = "Poptart";
		this->itemCost = 50;
	}
};

class PlainBase : public Poptart
{
public:
	PlainBase(void)
	{
		this->product_description = "Plain Base";
		this->itemCost = 100;
	}

};

class SpicyBase : public Poptart
{
public:
	SpicyBase(void)
	{
		this->product_description = "Spicy Base";
		this->itemCost = 150;
	}
};

class ChocolateBase : public Poptart
{
public:
	ChocolateBase(void)
	{
		this->product_description = "Chocolate Base";
		this->itemCost = 200;
	}
};

class CoconutBase : public Poptart
{
public:
	CoconutBase(void)
	{
		this->product_description = "Coconut Base";
		this->itemCost = 200;
	}
};

class FruityBase : public Poptart
{
public:
	FruityBase(void)
	{
		this->product_description = "Fruity Base";
		this->itemCost = 200;
	}
};

///////////////////////////////////////END OF Bases DECORATOR PATTERN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//////////////////////////////////////Fillings DECORATOR PATTERN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

class Filling : public Product // filling also inherits from product, so the name and cost can be polymorphed for the future fillings
{
private:
	Product* product; // creates a compositional relationship between the product and filling class
	int filling_cost;
	string filling_desc;
public:
	virtual void addToPoptart(Product*); // this declaration would add to the current base selection
	virtual int cost(void);
	virtual string description(void);
};

int Filling::cost(void) {
	return this->itemCost + product->cost(); // returns the item cost so the chocolate filling itemCost would return 20W
}

string Filling::description(void) {
	return product->description() + " & " + this->product_description; // sets the decription to the returned selection and the filling name description
}

void Filling::addToPoptart(Product* currentProduct) {

	product = currentProduct;

	
}

class ChocolateFilling : public Filling // classes for each filling which inherit from the filling class
{
public:
	ChocolateFilling(void) {
		product_description = "Chocolate Filling"; // sets the product description variable to the filling name
		itemCost = 20; // sets the item cost variable to the item's cost
	}
};

class BananaFilling :public Filling
{
public:
	BananaFilling(void) {
		product_description = "Banana Filling";
		itemCost = 50;
	}
};

class StrawberryFilling : public Filling
{
public:
	StrawberryFilling(void) {
		product_description = "Strawberry Filling";
		itemCost = 50;
	}
};


class RasberryFilling : public Filling
{
public:
	RasberryFilling(void) {
		product_description = "Raspberry Filling";
		itemCost = 50;
	}
};

class AppleFilling : public Filling
{
public:
	AppleFilling(void) {
		product_description = "Apple Filling";
		itemCost = 50;
	}
};

class BlackBerryFilling : public Filling
{
public:
	BlackBerryFilling(void) {
		product_description = "Blackberry Filling";
		itemCost = 50;
	}
};

class MapleFilling : public Filling
{
public:
	MapleFilling(void) {
		product_description = "Maple Filling";
		itemCost = 100;
	}
};

class MarshmallowFilling : public Filling
{
public:
	MarshmallowFilling(void) {
		product_description = "Marshmallow Filling";
		itemCost = 20;
	}
};

class CheeseFilling : public Filling
{
public:
	CheeseFilling(void) {
		product_description = "Cheese Filling";
		itemCost = 70;
	}
};

class CheeseAndHamFilling : public Filling
{
public:
	CheeseAndHamFilling(void) {
		product_description = "Cheese and Ham Filling";
		itemCost = 100;
	}
};

class CaramelFilling : public Filling
{
public:
	CaramelFilling(void) {
		product_description = "Caramel Filling";
		itemCost = 20;
	}
};

class VanillaFilling : public Filling
{
public:
	VanillaFilling(void) {
		product_description = "Vanilla Filling";
		itemCost = 50;
	}
};

/////////////////////////////////////////// END OF FILLINGS DECORATOR PATTERN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//////////////////////////////////////Poptart Dispenser \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

class Poptart_Dispenser : public StateContext, public Transition // this inherits from the statecontext and Transition, allowing for indirect state changes, thus allowing more states and transitions to be added
{
	friend class DispensesPoptart; // this would handle subtraction, so it needs to make direct changes, so a friend class would be beneficial for this
	friend class HasCredit;
private:
	PoptartState* PoptartCurrentState = nullptr; // set to null pointer by default
	bool itemDispensed = false;
	//indicates whether a product is there to be retrieved
	Product* DispensedItem = nullptr; // the dispensed item is set to null pointer by default so later on it can be set to one of the product bases
	bool itemRetrieved = false; //indicates whether a product has been retrieved
public:
	Poptart_Dispenser(int inventory_count);
	~Poptart_Dispenser(void);
	bool insertMoney(int money);
	bool makeSelection(int option); // option code gets passed in parameter so user can specify option code
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
	Product* getProduct(void);
	virtual void setStateParam(stateParameter SP, int value);
	virtual int getStateParam(stateParameter SP);
};


class OutOfPoptart : public PoptartState // this inherits from the poptart state, which is inheriting from the poptart dispenser, so changes made would be easier
{
public:
	OutOfPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class NoCredit : public PoptartState
{
public:
	NoCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class HasCredit : public PoptartState
{
public:
	HasCredit(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

class DispensesPoptart : public PoptartState
{
public:
	DispensesPoptart(StateContext* Context) : PoptartState(Context) {}
	bool insertMoney(int money);
	bool makeSelection(int option);
	bool moneyRejected(void);
	bool addPoptart(int number);
	bool dispense(void);
};

/////////////////// OUT OF POPTART (CONCRETE STATE SUBCLASS METHODS) \\\\\\\\\\\\\\\\\\\\\

bool OutOfPoptart::insertMoney(int money) {

	cout << "Error! Out of Poptarts!" << endl;
	this->moneyRejected();
	return false;
}

bool OutOfPoptart::makeSelection(int option) {

	cout << "Error! Out of Poptarts! No Selection Made!" << endl;
	return false;
}

bool OutOfPoptart::moneyRejected(void) { // transition in the OutOfPoptart State so must return true

	cout << "Returning Money!" << endl;
	this->CurrentContext->setStateParam(Credit, 0);
	this->CurrentContext->setState(Out_Of_Poptart); // setting state context to Out_Of_Poptart for clarity
	return true;
}

bool OutOfPoptart::addPoptart(int number) {

	cout << "Adding " << number << " Poptarts..." << endl; 
	number += CurrentContext->getStateParam(No_Of_Poptarts); // adds onto the current number of poptarts, then it gets set the the amount that was added on
	this->CurrentContext->setStateParam(No_Of_Poptarts, number);
	if (this->CurrentContext->getStateParam(No_Of_Poptarts) > 0) {

		this->CurrentContext->setState(No_Credit);
	}
	return true;
}

bool OutOfPoptart::dispense(void) {

	cout << "Error! No Poptarts left! Unable to Dispense!" << endl;
	return false;
}

/////////////////// NO CREDIT (CONCRETE STATE SUBCLASS METHODS) \\\\\\\\\\\\\\\\\\\\\


bool NoCredit::insertMoney(int money) {
	// Inserts the credit and sets the state parameter to have the added credit which gets carried over to the next state
	cout << "Inserting " << money << " Credits..." << endl; 
	money += CurrentContext->getStateParam(Credit);
	this->CurrentContext->setStateParam(Credit, money);
	this->CurrentContext->setState(Has_Credit);
	return true;
}

bool NoCredit::makeSelection(int option) {

	cout << "Error! No Credit Inserted!" << endl;
	return false;
}

bool NoCredit::moneyRejected(void) {

	cout << "Error! No Money Inserted!" << endl;
	return false;
}

bool NoCredit::addPoptart(int number) {
	cout << "Error! Poptarts already added!" << endl; 
	return false;
}

bool NoCredit::dispense(void) {

	cout << "Error! Not Enough Credit to Dispense!" << endl;
	return false;
}


/////////////////// HAS CREDIT (CONCRETE STATE SUBCLASS METHODS) \\\\\\\\\\\\\\\\\\\\\


bool HasCredit::insertMoney(int money) {

	cout << "Inserting " << money << "Credits" << endl; 
	money += CurrentContext->getStateParam(Credit); // gets the money and adds it onto credit
	this->CurrentContext->setStateParam(Credit, money); // sets the money so it is the same as credit so credit gets updated depending on how much credit the user inserted

	cout << "Current credit: " << CurrentContext->getStateParam(Credit) << endl; // displays the credit after the money has been added

	this->CurrentContext->setState(Has_Credit); // the state would be set to Has_Credit, as the user has inserted money

	return true;
}

bool HasCredit::makeSelection(int option) { // the option gets passed in the parameter in main, so the relevant option would be display and the cost would subtract etc.

	// sets to null pointer by default, but will be able to initialise new objects
	Filling* filling = nullptr;

	if (option & Base_Plain) {  // checks all the bits in options and the bits of the enum for base plain
		option -= Base_Plain; // subtracts the enum value so if it becomes 0, then the selection will be made
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = new PlainBase; // if option is true and the selected Base is true, then the base object would be set from nullptr to the selected Base
	}

	else if (option & Base_Spicy) {
		option -= Base_Spicy;
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = new SpicyBase;
	}

	else if (option & Base_Chocolate) {
		option -= Base_Chocolate;
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = new ChocolateBase;
	}

	else if (option & Base_Coconut) {
		option -= Base_Coconut;
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = new CoconutBase;
	}

	else if (option & Base_Fruity) {
		option -= Base_Fruity;
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = new FruityBase;
	}


	///// FILLINGS \\\\\

	while (option != 0) // while the option is not equal to 0 as its subtracting
	{
		
		if (option & Filling_Choco) // due to bitmasking, the filling can be added onto the base in main, for example: input 33 in the parameters of make selection, so plain base and chocolate filling will be chosen
		{
			option -= Filling_Choco; // subtracts the Filling_Choco enum from the option, if it's 0, then the options will display
			filling = new ChocolateFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem); // adds to the current base selection
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling; // sets the dispensed item to the filling which has been added to the poptart
		}

		else if (option & Filling_Banana)
		{
			option -= Filling_Banana;
			filling = new BananaFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;

		}

		else if (option & Filling_Strawberry)
		{
			option -= Filling_Strawberry;
			filling = new StrawberryFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Raspberry)
		{
			option -= Filling_Raspberry;
			filling = new RasberryFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Apple)
		{
			option -= Filling_Apple;
			filling = new AppleFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Blackberry)
		{
			option -= Filling_Blackberry;
			filling = new BlackBerryFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Maple)
		{
			option -= Filling_Maple;
			filling = new MapleFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Marshmallow)
		{
			option -= Filling_Marshmallow;
			filling = new MarshmallowFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Cheese)
		{
			option -= Filling_Cheese;
			filling = new CheeseFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Cheese_And_Ham)
		{
			option -= Filling_Cheese_And_Ham;
			filling = new CheeseAndHamFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Caramel)
		{
			option -= Filling_Caramel;
			filling = new CaramelFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}

		else if (option & Filling_Vanilla)
		{
			option -= Filling_Vanilla;
			filling = new VanillaFilling;
			filling->addToPoptart(((Poptart_Dispenser*)CurrentContext)->DispensedItem);
			((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling;
		}
	}

	cout << "Making Selection..." << endl;

	if (filling)
	{
		((Poptart_Dispenser*)CurrentContext)->DispensedItem = filling; // checks if the filling is used, if it is, then it would subtract the cost and set the dispensed item etc, if not then it'll just use the base
		cout << filling->description() << " Chosen!" << endl;
		cout << "Adding Filling..." << endl;
	}
	else {
		cout << ((Poptart_Dispenser*)CurrentContext)->DispensedItem->description() << " chosen!" << endl;// gets the description based on the option the user chose and displays it
	}

	this->CurrentContext->setState(Dispenses_Poptart); // state gets set to Dispenses_Poptart after the user has made the selection

	return true;
}

bool HasCredit::moneyRejected(void) {
	cout << "Money Rejected!" << endl;
	this->CurrentContext->setStateParam(Credit, 0); // the money gets ejected, hence why the credit gets set to 0
	this->CurrentContext->setState(No_Credit); // sets the state to No_Credit in accordance to the state diagram
											  
	return true;
}

bool HasCredit::addPoptart(int number) {
	cout << "Error! Unable to add poptarts!" << endl;
	return false;
}

bool HasCredit::dispense(void) {

	cout << "Error! Unable to Dispense!" << endl;
	return false;
}

/////////////////// DISPENSE POPTART (CONCRETE STATE SUBCLASS METHODS) \\\\\\\\\\\\\\\\\\\\\



bool DispensesPoptart::insertMoney(int money) { // this follows the state pattern, there is no transition for inserting money in dispense

	cout << "Error! Unable to Insert Money!" << endl;
	return false;
}

bool DispensesPoptart::makeSelection(int option) {

	cout << "Error! Selection Already Made!" << endl;
	return false;
}

bool DispensesPoptart::moneyRejected(void) {

	cout << "Error! Unable to Reject Money!" << endl;
	return false;
}

bool DispensesPoptart::addPoptart(int number) {

	cout << "Error! Unable to Add Poptarts" << endl;
	return false;
}

bool DispensesPoptart::dispense(void) {

	if (this->CurrentContext->getStateParam(Credit) < this->CurrentContext->getStateParam(Cost_Of_Poptart)) { // checks if user has enough credit for the poptart

		cout << "Error! Not enough credit! Please insert more credit!" << endl;
		this->CurrentContext->setState(Has_Credit); // state gets set to Has_Credit as that is a transition in the state diagram
		return true;
	}
	// if Poptarts == 0, then set state to Out_Of_Poptarts
	if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0) { // checks if there is enough poptarts for the selection
		cout << "Error! Cannot Dispense! No Poptarts in dispenser!" << endl;
		this->CurrentContext->setState(Out_Of_Poptart); // state gets set to Out_Of_Poptart as that is a transition in the state diagram

		return true;
	}

	else { // this runs if user has enough credits for the poptart and there are enough poptarts

		this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - this->CurrentContext->getStateParam(Cost_Of_Poptart)); // sets the state parameter to the credit - the cost so the current credit would be the leftover credits

		this->CurrentContext->setStateParam(No_Of_Poptarts, this->CurrentContext->getStateParam(No_Of_Poptarts) - 1); // subtracts the current amount of poptarts by 1, then sets the value to the remaining poptarts

		cout << "Dispensing " << ((Poptart_Dispenser*)CurrentContext)->DispensedItem->description() << endl; // gets the description and displays it
		cout << "Current Credit: " << this->CurrentContext->getStateParam(Credit) << endl; // checks the remaining credit, as the enum is used reference the credit
		cout << "Poptarts Remaining: " << this->CurrentContext->getStateParam(No_Of_Poptarts) << endl; // checks the remaining poptarts, as the enum is used reference the amount of poptarts
		if (this->CurrentContext->getStateParam(No_Of_Poptarts) == 0) { // checks if there are no poptarts left after subtracting so the state can be set to Out_Of_Poptart
			cout << "No Poptarts left!" << endl;
			this->CurrentContext->setState(Out_Of_Poptart);
			return true;
		}
		

		if (this->CurrentContext->getStateParam(Credit) > 0) { // checks if there is leftover credit, if there is credit leftover, then state is set to Has_Credit, otherwise it gets set to No_Credit

			this->CurrentContext->setState(Has_Credit);
			return true;
		}
		else {
			this->CurrentContext->setState(No_Credit);
			return true;
		}
	}

}

///////////////////// END OF CONCRETE STATE SUBCLASSES \\\\\\\\\\\\\\\\\\\\\

Poptart_Dispenser::Poptart_Dispenser(int inventory_count) { // Constructor for the state machine

	this->availableStates.push_back(new OutOfPoptart(this)); // creates a new instantiation of the states and pushes them onto the vector
	this->availableStates.push_back(new NoCredit(this));
	this->availableStates.push_back(new HasCredit(this));
	this->availableStates.push_back(new DispensesPoptart(this));
	this->stateParameters.push_back(0); // state parameters are for costs, stock and credit
	this->stateParameters.push_back(0);
	this->stateParameters.push_back(0);

	// Available states are pushed back in the constructor thus allowing for more states to be pushed back which allows for more flexibility in adding new states

	this->setState(Out_Of_Poptart); // sets starting state to Out_Of_Poptart
	if (inventory_count > 0) {
		this->addPoptart(inventory_count);
	}

}

Poptart_Dispenser::~Poptart_Dispenser(void)
{
	if (!this->itemRetrieved)
	{
		delete this->DispensedItem; // deletes the dispensed item
	}
}

bool Poptart_Dispenser::insertMoney(int money)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->insertMoney(money);  // returns the money the user inserted so the pointer can be used later on for the passing the value through parameter
}

bool Poptart_Dispenser::makeSelection(int option)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->makeSelection(option);
}

bool Poptart_Dispenser::moneyRejected(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)
{
	PoptartCurrentState = (PoptartState*) this->CurrentState;
	return this->PoptartCurrentState->dispense();
}

Product* Poptart_Dispenser::getProduct(void)
{
	if (this->itemDispensed)
	{
		this->itemDispensed = false;
		this->itemRetrieved = true;
		return this->DispensedItem;
	}

	return nullptr;
}
void Poptart_Dispenser::setStateParam(stateParameter SP, int value)
{
	if (SP == Cost_Of_Poptart) return;
	this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)
{
	if (SP == Cost_Of_Poptart)
	{
		if (DispensedItem == nullptr) return 0; // returns nothing if there is no dispensed item
		return DispensedItem->cost(); // this would return the cost of the dispensed item, so the 
	}
	return this->stateParameters[SP];
}
