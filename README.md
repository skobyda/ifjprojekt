# ifjprojekt

Pokyny pre členov môjho tímu. Úplne základy gitu:

1. Ako si stiahnuť tento projekt:
* v správnom adresári zadajte v linux termináli command:
```
$ git clone https://github.com/skobyda/ifjprojekt/
```
* teraz by ste mali mať priečinok s nazvom ifjprojekt, kde sa nachádza náš projekt.

2. Pred tým ako začnete programovať (budeme to nazývať riešenie problému):
* Pre každú odlišný problém, ktorý chcete vykonávať na projekte si vytvoríte branch (vetev):
```
$ git checkout master
$ git pull
$ git branch nazov_branche
$ git checkout nazov_branche
```
* Akékoľvek zmeny, ktoré spravíte v tejto vetvy, sa neprejavia v iných vetviach.

3. Ako programovať:
* Spravte potrebné zmeny v projekte. Zmeny ktoré píšete a ukladate do súborov, sa budú nachádzať len
na vašom počítači. Na githube sa zatiaľ nachádzať nebudú.
* Keď si myslíte, že ste spravili ucelený fungujúci postup na riešení vašeho problému, spravíte commit.
```
$ git add subor_na_ktorom_ste_pracovali.c
```
* git add môžete spraviť nad toľkými súbormi, ktoré chcete pridať do commitu.
```
$ git commit
```
* commit by mal zahrnovať funkčný pokrok v riešení vašeho problému.

4. Keď už ste daný problém vyriešili a skončili ste s programovaním toho na čo bola vaša vetev určená:
* Pushnete všetky svoje commity na github. Vďaka tomu členovia vášho tímu si môžu stiahnuť vaše zmeny
do svojho počítača a pracovať na nich.
```
$ git push origin nazov_branche
```
* ďalej, ak chcete pracovať na inom probléme, tak sa budete znovu riadiť od kroku číslo 2.
