

//klasa gry dziedziczaca po abstrakcyjnej grze
class CaveGame : public pe::Game { //pe - penetration engine

//tu moga byc pola odpowiedzialne za logike gry, jakies punkty etc
	Rocket* rocket = NULL; //rakieta, tak w C++ sie nie da zwyklym (C++x11 da sie)
	Camera* mainCamera = NULL; //kamera glowna
	HUD* hud = NULL; //HUD (ekran z głupimi rzeczami typu nie wiem menu punkty etc)
	vector<Enemy*> enemies; //wektor jakichs obiektow przeciwnikow

public:
	//metody wydaje mi sie o dostepie publicznym, aczkolwiek niekoniecznie ale moze lepiej niech tak bedzie.
	//virtualne z oczywistych powodow (klasa abstrakcyjna)

	virtual initializeGame(pe::GameConfig* gc) //GameConfig - klasa odpowiedzialna za posiadanie konfiguracji gry
	{
		//ladowanie obiektow z OBJ, jakies sringi renderingi, ladowanie zapisanych gier, cokolwiek dusza zapragnie
		this->setGameResolution(gc->getConfig("resX"), gc->getConfig("resY"))); // ustawiam sobie rozdzielczosc gry taka jak w konfiguracji
	}

	//nie moze zabraknac inicjalizacji sceny 3D - ktora uruchamia sie przy ladowaniu sceny (troche inne niz ladowanie np HUD samego)
	virtual initializeScene(pe::GameConfig* gc, pe::Scene* scene)
	{
		rocket = new Rocket(); //buduje klase Rocket (ktora dziedziczy z bazowej klasy reprezentujaca zwykly obiekt)
		hud = new HUD(); //buduje obiekt HUD dziedziczacy z pe::HUD? 
		rocket->setMesh(this->getMeshFactory()->get("resources/obj/rocket.obj"));
		rocket->addChild(camera); //ciekawy zabieg - dodanie kamery jako obiektu dziecka, genialna sprawa
		scene->setMainCamera(camera); //ustawienie camery jako kamery aktualnej (zawsze mozna zmienic)
		scene->addHUD(hud);
		scene->addObject(rocket); //dodajemy rakiete do sceny
	}

	//nadpisana metoda virtualna jak wszystkie inne, obsluguje zdarzenia uzytkownika
	virtual void handleUserEvent(Event* event)
	{
		//zdarzenia z interakcji z uzytkownikiem
		if(event->isKeyboardEvent())
			handleKeyboard(event);
		if(event->isMouseClickEvent())
			handleMouseClick(event);
		if(event->isMouseMove())
			handleMouseMove(event);
	}

	void handleMouseMove(pe::Event e)
	{
		this->getActiveCamera()->rotate(); //jakos to obsluzyc, kamera paczy w inna strone np
	}
};

//obiekt Timer jest obiektem klasy bazowej ktory dodaje siebie samego w konstruktorze do kontenera timerow, ktory wykonuje dane timery w danym czasie
//chyba to nie jest thread-safe ale wystarczy nam w zupelnosci do podstawowych funkcjonalnosci
class MoveTimer : public Timer 
{
protected:
	Rocket* rocket;
	Interval interval;
public:
	MoveTimer(Rocket* rocket, Interval interval)
		: Timer()
	{
		this->rocket = rocket;
		this->interval = interval;
	}

	virtual void click()
	{
		rocket->move(100, 0, 0);
	}
};

//klasa rakieta nadpisana nie wiadomo czemu na razie, ale na pewno ma to jakis sens w odniesieniu do zarzadania logika takiej rakietki

class Rocket : public pe::GameObject
{

};

class Enemy : public pe:GameObject
{
protected:
	EnemyStrategy* es;
public:
	Enemy(EnemyStrategy* es)
	{
		this->es = es; //inicializacja enemystrategy
	}

	virtual void move()
	{
		es->move(this);
	}
};

//klasa odpowiedzialna za strategie ruchu danego przeciwnika
class EnemyStrategy
{
public:
	virtual void move(Enemy* enemy) = 0; //te zerowanie to inicjalizowanie pustej metody (klasa abstrakcyjna/interfejs)
};

//konkretna strategia ruchu - poruszanie sie tylko w OZ o zadanej wartosci przekazanej przez konstruktor
//USAGE:
//new Enemy(StraightEnemyStrategy(1)); //tworzy przeciwnika z ta strategia o predkosci 1
class StraightEnemyStrategy : public EnemyStrategy
{
protected:
	double speed;
public:
	StraightEnemyStrategy(double speed)
	{
		this->speed  = speed;
	}

	virtual void move(Enemy* es)
	{
		enemy->move(0,0,speed); //ruch o zadana wartosc
	}
};