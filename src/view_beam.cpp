/////////////////////////////////////////////////////////////////////////////
// Name:        view_beam.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Author and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "view.h"

//----------------------------------------------------------------------------

#include <algorithm>
#include <math.h>

//----------------------------------------------------------------------------

#include "beam.h"
#include "doc.h"
#include "layer.h"
#include "layerelement.h"
#include "note.h"
#include "staff.h"

namespace vrv {

/*
#define BEAMEDIT 50	// code arbitraire identifiant la structure de debordement pour beams
typedef struct BeamEdit {	
    short iHauteur;
    float fPente;
} BeamEdit;
*/
//static BeamEdit BeamEd, *ptBeamEd;

/*
void traiteQueue (int *hautqueue, Element *chk)
{	ptdebord = (Debord *)chk->pdebord;
	ptdebord ++;	// pointe sur valeurs
	ptBeamEd = (BeamEdit *)ptdebord;
	*hautqueue += ptBeamEd->iHauteur;
	return;
}
*/

/* position x, pos. curseur yy, decalage y sous curseur, rayon de note;
   collecte et calcule les donnees */

#define NbFRACTURES 20		/* nombre de '+' possibles */
#define NbREL 80		/* nombre de valeurs reliees possibles */
#define MAX_MIF 20	/* nombre max de sous-groupes de beams */


static struct coord {  float a;
			float b;
			unsigned vlr: 8;	/* valeur */
			unsigned prov: 8;	/* ON si portee sup. */
            LayerElement *chk;
	     } 	crd[NbREL]; /* garde les coord.d'entree*/

/**
 * This structure is used for calculating the beam internally.
*/
struct BeamFlag {	unsigned beam_chrd:3;	/* retournee par beam() a input() */
		unsigned beamchrd:3;	/* accord() et note() */
		unsigned inpt:1;	/* usage in input(),beam(),accord() */
		unsigned markchrd:1;	/* in beam() */
        unsigned niaf:8;
};
struct BeamFlag bch;
double sy_up = 0.0;
float hauteurBarreMoyenne = 3.0;
double dA, dB;

/* This need to be put into a beam class */

void View::DrawBeam(  DeviceContext *dc, Layer *layer, Beam *beam, Staff *staff )
{
    LayerElement *chk;
	static struct fb {
		unsigned _liaison : 1;	/* temoin pour liaison: si ON, il y a
							   de la liaison dans l'air et beam doit
							   doit appeler liaison_note. Cfr.liais_note()*/
		unsigned mrq_port : 2;	/* ON si changement in yy; necessaire
								pour decider beams lorsque 2 portees */
		unsigned mq_val   : 1;	/* marqueur de changement de valeur*/
		unsigned fl_cond : 1;	/* flags concernant partage portees */
		unsigned flsht    : 3;	/* garde le pnt->_shport */
		unsigned _grp	  : 1;	/* marqueur de groupes rythmiques */ // unused in Verovio
        data_STEMDIRECTION dir	  : 3;	/* marqueur direction queues */
	}	fb;
	int iHauteur=0;
	float fPente = 0.0;
	int ct;			/* compteur d'entree */
	int _ct;		/* compte d'entree moins 1 */
	int cpte_stop;	/* compteur de st_rl */
	unsigned st_rl[NbFRACTURES];	/* garde les ruptures de beams */
	int _yy[2];		/* garde dernier yy pour test mrq_port*/
	static int _ybeam[NbREL];	/* garde les sommets de queues  */
	int shortest, valref;	/* garde valeur la plus breve
				* m_dur. de reference */
	double high;
	double low;		/* gardent les y extremes */
	double dx[2];			/* rayon / 2 */
	double y_moy;		/* calcul de moyenne y pour dir */
	double s_x=0.0, s_y=0.0, s_xy=0.0, s_x2=0.0, s_y2=0.0;	/* sommes pour la regress. */
	double xr;				/* variables de travail */
	double delt_y, barre_y, ecart, y1;
	double sy_dec_moy, sy_dec[2];
	double fbarre_y;
	int mx_i[MAX_MIF], mx_f[MAX_MIF], my_i[MAX_MIF], my_f[MAX_MIF];
	char m_i[MAX_MIF];	/* pour stocker les prov des marqueurs */
	int fx1,fx2,fy1,fy2;		/* coord. */
	float milieu;

	int i, j, t, h, k, si;		/* compteurs de boucles */
	int _mif, decalage;
	int valtest;			/* travail */
	int apax;	 	/* test pour valeur breve isolee en debut  de groupes */
	//int provshp=0;
	int deltabar, deltanbbar, deltablanc;
	
    beam->ResetList( beam );
    
    // Should we assert this at the beginning?
    if (beam->m_list.empty())
        return;
    
    // chk point to the first Note in the layed out layer
    chk = dynamic_cast<LayerElement*>(beam->m_list.front());
    
    //	bch.markchrd = shortest = fb.mq_val = valref = ct = cpte_stop = fb.mrq_port = OFF;
	bch.markchrd = 0;
	shortest = 0;
	fb.mq_val = 0;
	valref = ct = cpte_stop = 0;
	fb.mrq_port = OFF;
	high = y_moy = sy_up = sy_dec_moy = 0.0;
	for (i=0; i<NbFRACTURES; i++)
		st_rl[i] = 0;
	/***if (e_t->_shport) { provshp = e_t->_shport; shportee (0);}***/
		/* retablir a la fin si provshp existe */

	low = chk->GetDrawingY();	/* initialiser */
    k = ((Note*)chk)->GetColored()==BOOLEAN_true ? ((Note*)chk)->GetDur()+1 : ((Note*)chk)->GetDur();
    
	valref = k;		/* m_dur test conservee */
    //	valref = chk->m_dur;		/* m_dur test conservee */
	fb.flsht = 0;

	if (staff->notAnc) {
		dx[0] = dx[1] = 0.0;
    } 
    else
    {	
        dx[0] =  m_doc->m_drawingNoteRadius[staff->staffSize][0];
        dx[1] =  m_doc->m_drawingNoteRadius[staff->staffSize][1];
        dx[0] -= (m_doc->m_env.m_stemWidth)/2;
        dx[1] -= (m_doc->m_env.m_stemWidth)/2;
    }
	_yy[0] = staff->GetDrawingY();

    ListOfObjects::iterator iter = beam->m_list.begin();
    
	do
	{
        
        //LogDebug("-> %s", chk->GetClassName().c_str() );
        
        if ( chk->IsNote() ) {
            k = ((Note*)chk)->GetColored()==BOOLEAN_true ? ((Note*)chk)->GetDur()+1 : ((Note*)chk)->GetDur();
        }

        // if (chk->type == NOTE && /*chk->sil == _NOT &&*/ k > DUR_4)
		if (k > DUR_4)
        {
			(crd+ct)->chk = chk;
			/* garantir uniformite des flags */

			/***if (!fb.mrq_port && chk->_shport) {
				fb.mrq_port = chk->_shport;
            }***/

			(crd+ct)->a = chk->GetDrawingX() + chk->m_hOffset - m_doc->m_env.m_stemWidth / 2;		/* enregistrement des coord. */
			(crd+ct)->vlr = k;
			if (chk->IsNote() && ((Note*)chk)->GetBreaksecInt() && ct)
                /* enregistr. des ruptures de beaming; des la 2e note;(autrement idiot)*/
				*(st_rl + (cpte_stop++)) = ct;

			/***if (extern_q_auto && chk->chord)
			{	bch.markchrd=ON;
				fb.flsht = fb.flsht ? fb.flsht : chk->_shport;
			}***/
            if (chk->IsNote())	// Žviter de prendre en compte silences
            {
                shortest = std::max(k,shortest);
                if (!fb.mq_val && k != valref) fb.mq_val = ON; /* plus d'une valeur est presente*/
                valref = std::min(k,valref);
            }
			ct++;
		}
        
        iter++;
        if (iter == beam->m_list.end()) {
            break;
        }
        
        chk = dynamic_cast<LayerElement*>(*iter);
		if (chk == NULL) { 
            return;
        }
        
	}	while (1);

    // SECURITE : EVITER DE BARRER UN ACCORD ISOLE...
/*	if (chk->IsNote() && (((Note*)chk)->m_chord & CHORD_TERMINAL)  && (chk->m_drawingX == layer->beamListPremier->m_drawingX))
	{	chk = layer->beamListPremier;
		do {	
                ((Note*)chk)->m_beam[0] = 0;
				chk = layer->GetNext(chk);
			}	while (chk && chk->IsNote() && !((Note*)chk)->m_chord & CHORD_TERMINAL);
		layer->beamListPremier = NULL;
		return;
	}
*/
    
    //LogDebug("ct %d", ct );

	_ct = ct - 1;		/* compte d'entree moins 1 */

	/* ici, verifier la provenance (haut/bas) des queues en cas de
		partage entre portees, et repasser la liste */
    /***
	if (fb.mrq_port)
	// le y le plus haut est dans _yy[0] 
	{	if (fb.mrq_port==1)
		{	_yy[0] = (this != phead) ? staff->ptr_pp->m_drawingY : staff->m_drawingY; 
			_yy[1] = staff->m_drawingY;
		}
		else
		{	_yy[1] = (this != ptail) ? staff->ptr_fp->m_drawingY : staff->m_drawingY; 
			_yy[0] = staff->m_drawingY;
		}
	}
    ***/
    
	for (i = 0; i < ct; i++)
	{	switch (fb.mrq_port)
		{
            case 0: crd[i].prov = OFF;
					(crd+i)->b = crd[i].chk->GetDrawingY();
					break;
            /**
			case 1: if (crd[i].chk->m_staffShift)
					{	crd[i].prov = ON;
						(crd+i)->b = crd[i].chk->m_drawingYRel + _yy[0];
					}
					else
					{	crd[i].prov = OFF;
						(crd+i)->b = crd[i].chk->m_drawingYRel + _yy[1];
					}
					break;
			case 2: if (crd[i].chk->m_staffShift)
					{	crd[i].prov = OFF;
						(crd+i)->b = crd[i].chk->m_drawingYRel + _yy[1];
					}
					else
					{	crd[i].prov = ON;
						(crd+i)->b = crd[i].chk->m_drawingYRel + _yy[0];
					}
             **/
		}
		high= std::max((double)(crd+i)->b,high);		/* enregistrement des extremes */
		low = std::min((double)(crd+i)->b,low);
        /* lie au choix, plus bas, d'introduire l'accelerateur pour sy_up...
        if ((crd+i)->b==high) highIndice = i;
        if ((crd+i)->b==low) lowIndice = i;
        */
		y_moy += crd[i].b;
	}
    /***
	if (provshp)
		shportee (provshp);
    ***/
    
	if (ct<2) {
        //layer->beamListPremier = NULL;
		return;		/* test erreur input */
    }


	fb.dir = layer->GetDrawingStemDir();
	if (!fb.mrq_port)
	{	
        milieu = _yy[0] - (m_doc->m_drawingInterl[staff->staffSize] * 2);
		y_moy /= ct;
        if (fb.dir == STEMDIRECTION_NONE) {
            if ( y_moy <  milieu ) fb.dir = STEMDIRECTION_up;
            else fb.dir = STEMDIRECTION_down;
        }
	}

    if (crd[_ct].chk->m_cueSize == false)
    {
        deltanbbar = m_doc->m_drawingBeamWidth[staff->staffSize];
        deltablanc = m_doc->m_drawingBeamWhiteWidth[staff->staffSize];
    }
    else
    {	deltanbbar = std::max(2, (m_doc->m_drawingBeamWidth[staff->staffSize]/2));
        deltablanc = std::max(2, (m_doc->m_drawingBeamWhiteWidth[staff->staffSize]-1));
    }
	deltabar = deltanbbar + deltablanc;


    /* Calcul des sommes et facteurs s_y, s_xy necessaires a la regression.
    La regression se base sur les sommets des queues, calcul‚s en ajoutant un
    ‚cart standard fonction de la valeur la plus breve enregistree. Direction
    connue, donc ajout ou retrait de l'ecart, et correction du x d'origine en
    fonction de la direction.*/

    /***
    if (crd[_ct].chk->existDebord)
    {	ptdebord = (Debord *)chk->pdebord;
        ptdebord ++;	// pointe sur valeurs
        ptBeamEd = (BeamEdit *)ptdebord;
        iHauteur = ptBeamEd->iHauteur;
        fPente = ptBeamEd->fPente;
        BeamEd = *ptBeamEd;
    }
    else ***/
    {	
        iHauteur = 0;
        fPente = 0.0;
    }

    /***
	if (fb.mrq_port && extern_q_auto)
	// deux portees concernees (partage), en mode automatique 
	{	ecart = e_t->m_doc->m_drawingInterl[staff->staffSize]*6;
		for (i=0; i<ct; i++)
		{	if ((crd+i)->prov)
			{	(crd+i)->a -= dx[crd[i].chk->dimin];
				*(_ybeam+i) = crd[i].b - ecart;
				crd[i].chk->m_stemDir = 0;
			}
			else
			{	(crd+i)->a += dx[crd[i].chk->dimin];
				*(_ybeam+i) = crd[i].b + ecart;
				crd[i].chk->m_stemDir = 1;
			}
			s_y += _ybeam[i];
 			s_x += crd[i].a;
			s_x2 += crd[i].a * crd[i].a;
			s_xy += crd[i].a * _ybeam[i];
		}
	}
	else ***/
    
	// une seule portee; on tient compte de la direction precedemment calculee*/
	{	
        ecart = ((shortest-DUR_8)*(deltabar));

		if (crd[_ct].chk->m_cueSize)
			ecart += m_doc->m_drawingHalfInterl[staff->staffSize]*5;
		else
        //   Le 24 Septembre 1993: obtenir des DUR_8 reliees a la hauteur des separees 
			ecart += (shortest > DUR_8) ? m_doc->m_drawingInterl[staff->staffSize]*hauteurBarreMoyenne : m_doc->m_drawingInterl[staff->staffSize]*(hauteurBarreMoyenne+0.5);

		if (fb.dir == STEMDIRECTION_down)
		{
            dx[0] = - dx[0];
			dx[1] = - dx[1];
		}
        /***
        if (crd[_ct].chk->existDebord) {
        ecart = m_doc->m_drawingInterl[0]*2;
            if (!fb.mrq_port) extern_q_auto= 0;
        }
        ***/

		ecart = (fb.dir == STEMDIRECTION_up ? ecart : -ecart);

		y_moy += ecart;
        if ((fb.dir == STEMDIRECTION_up && y_moy <  milieu) || (fb.dir == STEMDIRECTION_down && y_moy > milieu)) {
			ecart += milieu-y_moy;
        }

		for (i=0; i<ct; i++)
		{
            *(_ybeam+i) = crd[i].b + ecart;
			(crd+i)->a +=  dx[crd[i].chk->m_cueSize];
			s_y += _ybeam[i];
 			s_y2 += _ybeam[i] * _ybeam[i];
			s_x += crd[i].a;
			s_x2 += crd[i].a * crd[i].a;
			s_xy += crd[i].a * _ybeam[i];
            if ( crd[i].chk->IsNote() ) {
                ((Note*)crd[i].chk)->m_drawingStemDir = fb.dir;
            }
		}

	}

	y1 = ct * s_xy - s_x * s_y;
	xr = ct * s_x2 - s_x * s_x;

	if (y1 && xr)	// securite: eviter division par 0 si deux x identiques
		dB = y1 / xr;
	else
		dB = 0.0;
	/* Correction esthetique : */
	if (fabs(dB) < m_doc->m_drawingBeamMinSlope ) dB = 0.0;
	if (fabs(dB) > m_doc->m_drawingBeamMaxSlope ) dB = (dB>0) ? m_doc->m_drawingBeamMaxSlope : - m_doc->m_drawingBeamMaxSlope;
	/* pente correcte: entre 0 et env 0.4 (0.2 a 0.4) */

if (fPente)
	dB += fPente;

	dA = (s_y - dB * s_x) / ct;


	h = cpte_stop ? 1 : (!fb.mq_val ? (shortest - DUR_4) : 1); /***((fb.mrq_port && extern_q_auto)?(valref-DUR_4):1));***/
		/* nombre de barres a dessiner */


	/* flag condition: pour eviter tests complexes repetes in boucles;
	 * concerne les cas de partages entre portees. Vrai si pas de stop-rel
	 * (i.e. possibilite de plusieurs barres communes traversantes) */

	fb.fl_cond = OFF;

    /***
	if (fb.mrq_port && extern_q_auto)
		fb.fl_cond = ON;	// independamment de mq_val
    ***/

	if (fb.fl_cond)
	/*valeur pour allonger queues si partage entre portees et mode autom.*/
	{
		i = deltabar * h - deltablanc;	/* nombre de points de dec */
		j = i / 2;
		sy_dec[0] = j;
		sy_dec[1] = -sy_dec[0];

		if ( i % 2)
			sy_dec[1] -= 1;
		sy_dec_moy = sy_dec[0] + abs (sy_dec[1]);
	}

    if (iHauteur)
        dA += iHauteur;

	/* calcul du ybeam des queues */
	for ( i=0; i<ct; i++ )
	{	xr = *(_ybeam+i);	/* xr, variable de travail */
		*(_ybeam+i)= dA + sy_up + dB * crd[i].a;
		if (fb.fl_cond)
			*(_ybeam+i) += sy_dec [crd[i].prov];
		
		/* test pour garantir l'absence de recoupement */
        if (!iHauteur)
            if (!fb.mrq_port)
            {	if ((fb.dir == STEMDIRECTION_up && xr > *(_ybeam+i)) || (fb.dir == STEMDIRECTION_down && xr < *(_ybeam+i)))
                {
                    sy_up += xr - *(_ybeam+i);
                    i = -1;	/* on refait la boucle avec un sy_up */
                }
            }	


	}

	/* dessin de la barre pilote et des queues */

	for (i=0; i<ct; i++)
	{
		if (fb.fl_cond)	/* esth: eviter que queues depassent barres */
		{	if (crd[i].prov)	/* venant du haut, m_stemDir en bas */
			{	/***fy1 = *(_ybeam+i)+v_pnt;***/	/* on raccourcit m_stemDir */
				fy2 = crd[i].b-m_doc->m_drawingVerticalUnit2[staff->staffSize];
			}
			else
			{	/***fy1 = *(_ybeam+i)-e_t->v_pnt;***/	/* on allonge m_stemDir */
				fy2 = crd[i].b+m_doc->m_drawingVerticalUnit2[staff->staffSize];
			}
		}
		else	// on tient compte de l'‚paisseur qui fait des "bosses"
		{	if (fb.dir == STEMDIRECTION_up)	// m_stemDir en haut
			{	fy1 = *(_ybeam+i) - m_doc->m_env.m_stemWidth;
				fy2 = crd[i].b+m_doc->m_drawingVerticalUnit2[staff->staffSize];
                crd[i].chk->m_drawingStemStart.x = crd[i].chk->m_drawingStemEnd.x = crd[i].a;
                crd[i].chk->m_drawingStemStart.y = fy2;
                crd[i].chk->m_drawingStemEnd.y = fy1;
                crd[i].chk->m_drawingStemDir = true;
			}
			else
			{	fy1 = *(_ybeam+i) + m_doc->m_env.m_stemWidth;
				fy2 = crd[i].b-m_doc->m_drawingVerticalUnit2[staff->staffSize];
                crd[i].chk->m_drawingStemStart.x = crd[i].chk->m_drawingStemEnd.x = crd[i].a;
                crd[i].chk->m_drawingStemStart.y = fy2;
                crd[i].chk->m_drawingStemEnd.y = fy1;
                crd[i].chk->m_drawingStemDir = false;
			}
		}
		if ((crd+i)->chk->IsNote())
		{	
            DrawVerticalLine (dc,fy2, fy1, crd[i].a, m_doc->m_env.m_stemWidth);
            
// ICI, bon endroit pour enfiler les STACCATOS - ne sont traités ici que ceux qui sont opposés à la tête (les autres, in wgnote.cpp)
			//if (((Note*)(crd+i)->chk)->m_artic
			//	 && (!((Note*)(crd+i)->chk)->m_chord || (((Note*)(crd+i)->chk)->m_chord & CHORD_TERMINAL)))
			// les cas non traités par note()
/*			{	if (fb.dir == STEMDIRECTION_up || (fb.mrq_port && m_stemLen && !crd[i].prov))
					putStacc (dc,crd[i].a-dx[crd[i].chk->dimin],fy1+e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, 0,crd[i].chk->typStac);
				else
					putStacc (dc,crd[i].a-dx[crd[i].chk->dimin],fy1-e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, -1,crd[i].chk->typStac);
			}
*/
			{	
                /***if (fb.mrq_port && extern_q_auto)
				{	if (crd[i].prov)
						putStacc (dc,crd[i].a+dx[crd[i].chk->dimin],fy1-e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, -1,crd[i].chk->typStac);
					else
						putStacc (dc,crd[i].a-dx[crd[i].chk->dimin],fy1+e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, 0,crd[i].chk->typStac);
				}
				else if (fb.dir == STEMDIRECTION_up)
					putStacc (dc,crd[i].a-dx[crd[i].chk->dimin],fy1+e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, 0,crd[i].chk->typStac);
				else
					putStacc (dc,crd[i].a-dx[crd[i].chk->dimin],fy1-e_t->m_doc->m_drawingInterl[staff->staffSize]-staff->m_drawingY, -1,crd[i].chk->typStac);
                ***/
			}

		}

	}

    // NOUVEAU
    // Correction des positions x extremes en fonction de l'‚paisseur des queues
	(*crd).a -= (m_doc->m_env.m_stemWidth-1) / 3;
	(crd+_ct)->a += (m_doc->m_env.m_stemWidth-1) / 3;

	delt_y = (fb.dir == STEMDIRECTION_down || fb.fl_cond ) ? 1.0 : -1.0;
	/* on choisit une direction pour faire le premier paquet horizontal */

	fy1 = *_ybeam; fy2 = *(_ybeam+_ct);


	if (fb.fl_cond)
    /* reequilibrage du tir horizontal: on aligne les beams sur les queues
        qui ont ete allongees selon leur direction pour couvrir l'epaisseur */

	{
		if (!crd[0].prov) fy1 -=  sy_dec_moy;
		if (!crd[_ct].prov) fy2 -= sy_dec_moy;
	}

	fx1 = (*crd).a; fx2 = (crd+_ct)->a;


	/* dessin des barres supplementaires jusqu'a concurrence de la valeur
	 *  minimum commune a tout le groupe: i.e. une seule s'il y a des "stoprel"
	 *  (fragmentation de beaming), ou sinon valref (ou h).
	*/
	/* chk->cone est le flag des cones d'acc. et ralent. */
	//s_y = crd[0].chk->cone ? 0.0 : delt_y; // removed in ax2
	//s_y2 = (crd+_ct)->chk->cone ? 0.0 : delt_y; // removed in ax2
	s_y = delt_y; // removed in ax2
	s_y2 = delt_y; // removed in ax2

	for (j=0; j<h ; j++)
	{
		decalage = deltanbbar*delt_y;
        DrawObliqueLine (dc,fx1,fy1,fx2,fy2, decalage /***, workColor2***/);
		fy1 += decalage; fy2 += decalage;

/* ici, redescendre  de l'epaisseur de la barre s'il y a accele */
		if (!s_y)
			fy1 += (deltanbbar * delt_y) * -1;
		else
			fy1 += s_y*deltablanc;
		if (!s_y2)
			fy2 += (deltanbbar * delt_y) * -1;
		else
			fy2 += s_y2*deltablanc;
	}

	/* calcul des x en cas de beaming multiple */
	/* parcours horizontal ajoutant barres en fonction de m_dur la plus 
	breve (shortest), controle par boucle while; la premiere boucle for
	controle le nombre d'etapes horizontales du parcours (par le nombre
	de commandes '+' enregistrees); la deuxieme boucle for teste pour
	chaque paquet entre deux '+' les valeurs; elle construit une array de 
	marqueurs partitionnant les sous-groupes; la troisieme boucle for est
	pilotee par l'indice de l'array; elle dessine horizontalement les barres 
	de chaque sous-groupe en suivant les marqueurs */ 

 
	/* cpte_stop=0 ? pas de rupture de beaming*/

    if (fb.mq_val || cpte_stop)	/* deuxieme partie du test */
    {
        valtest = DUR_8 + h;

        if ( fb.fl_cond )
        {	barre_y = deltablanc + sy_dec_moy;
            /* = decalage entre bout de m_stemDir et position acquise des barres */
        }
        else	/* pas de partage entre portees */

            barre_y = deltabar;

        /* h contient nombre de barres communes deja dessinees */
        if (fb.dir == STEMDIRECTION_up)	/* queues ascendantes: on descend */
            barre_y = -barre_y;

        while (valtest <= shortest)
        {	t = 0; si = 0;
            for (i=0; i<=cpte_stop; i++)	/* 1e boucle for */
            {	h = (*(st_rl+si) ? *(st_rl+si) : ct);
                /* var. test controlant 2e boucle for suivante
                 * en fonction du compteur de "+" */
    /*ici, t=j, i.e. increment des pos. de notes. On s'occupe maintenant de
    l'ensemble des valeurs contenues dans un groupe marque par cpte_stop. C'est
    la qu'il faut changer les signes des valeurs delta d'increment vertical.*/

                for(k=0; k<MAX_MIF; k++)	/* initialisation*/
                {	mx_i[k]=my_i[k]=mx_f[k]=my_f[k]=0;}

                for (j=t,_mif=0,m_i[_mif]=0; j < h; j++)	/* 2e boucle. */
                {	/* j<h : st_rl est trop loin de un cran */

                /* Ici, d‚cision si SILENCE doit ou non avoir des barres; si oui, ligne
                    suivante (condition: il doit etre pris aussi dans les crd plus haut):*/
                    if (((crd+j)->vlr) >= (unsigned int)valtest)	
                /*	si NON, alors: */
                    // if (((crd+j)->vlr) >= valtest && (crd+j)->chk->sil == _NOT)	
                    {
                        /*place marqueurs pour m_dur.egales/superieures
                         * a valtest en cours */
                        mx_f[_mif] = crd[j].a; 
                        my_f[_mif] = *(_ybeam+j);
                        if(!mx_i[_mif])
                        {	mx_i[_mif] = crd[j].a;
                            my_i[_mif] = *(_ybeam+j);
                            if (!_mif) apax = j;
                            if (!crd[j].prov)
                            /* enregistre les cas ou delta y est neg.*/
                                m_i[_mif] = 1;
                        }
                    }
                    /* rupture de chaine: on passe a un 2e groupe
                     * de marqueurs */
                    else if(mx_i[_mif])
                    {	_mif++;	/*incr. s'il y a un marq.*/
                        m_i[_mif] = 0;
                    }
                }

                fbarre_y = barre_y;	/* stockage */
                for (k=0; k<=std::min((mx_f[_mif]?_mif:(_mif-1)),MAX_MIF); k++)
                {
                    /* "constantes" de corr. definissant origine du calcul des
                     * y, dans les cas de partage entre portees automatiques;
                     * (construction anterieure en montant si fl_cond) */
                    if ( fb.fl_cond)
                    {	barre_y = abs(fbarre_y); delt_y = abs (delt_y);

                        if (m_i[k])		/* i.e. portee inf. (!crd[j].prov) */
                        {	barre_y = -barre_y;
                            delt_y = -delt_y;
                            sy_up = sy_dec[0];	/* valeur positive */
                        }

                        else
                            sy_up = sy_dec[1];	/* valeur negative */

                    }


                    /* on passe en revue, horizontalement, les marqueurs
                     * enregistres pour ce  groupe, en s'assurant que le
                     * max MAX_MIF n'est pas depasse */
                    if (mx_i[k] == mx_f[k])		/* une seule position concernee */
                    {
                        if (apax == t && k==0 && mx_i[k] != crd[_ct].a)	/* au debut du paquet */
                        {	fy1 = my_i[k] + barre_y;
                            mx_f[k] = mx_i[k] + m_doc->m_drawingLedgerLine[staff->staffSize][0];
                            fy2 = dA + sy_up + barre_y + dB * mx_f[k];

                            decalage= deltanbbar*delt_y;
                            DrawObliqueLine (dc,mx_i[k],fy1,mx_f[k],fy2, decalage /***, workColor2***/ );
                            fy1 += decalage; fy2 += decalage;

                        }
                        else		/* corps ou fin de paquet */
                        {	fy2 = my_i[k] + barre_y;
                            mx_i[k] -= m_doc->m_drawingLedgerLine[staff->staffSize][0];
                            fy1 = dA + sy_up + barre_y + dB * mx_i[k];
                            decalage = deltanbbar*delt_y;
                            DrawObliqueLine (dc,mx_i[k],fy1,mx_f[k],fy2, decalage  /***,workColor2***/);
                            fy1 += decalage; fy2 += decalage;

                        }
                    }
                    else if (mx_i[k])		/* s'il y a un marqueur */
                    {	fy1 = my_i[k] + barre_y;
                        fy2 = my_f[k] + barre_y;
                        decalage = deltanbbar*delt_y;
                        DrawObliqueLine (dc,mx_i[k],fy1,mx_f[k],fy2, decalage /***,workColor2***/);
                        fy1 += decalage; fy2 += decalage;

                    }				
                }	/* fin de boucle testant sous-ensembles marques _mif*/

                if ( fb.fl_cond)	/* retablissement des valeurs positives */
                {	barre_y = abs(fbarre_y);
                    delt_y =  abs(delt_y);
                }

                if (*st_rl)
                {	si++; t = j;} 
                else t = 0;
            }			/* fin de premiere boucle for */

            valtest += 1;	/* increments de valeur et d'espace */
            barre_y += delt_y*deltabar;
        }			/* fin de boucle while */
    }				/*fermeture de la deuxieme partie du test */

	/***beamPremier = layer->beamListPremier;***/
	//layer->beamListPremier = NULL; RZ COMMENTED

    /***
	if (fb._grp)	// group.fin)
	{	e_t->rel = OFF; 
		rythmeInf(dc);
	}
    ***/


	if (fb._liaison)
		return;	

/*	1111, code retourne pour appeler liais_note (NULL) apres beam(): on
    evite ainsi le risque d'appel recursif de beam. C'est le code de la
	 liaison "retardee": la direction des
	queues n'etait pas encore connue de liais_note lorsque la liaison a
	ete demandee, puisqu' elle est calculee (en mode automatique) par
	la presente fonction (et placee dans le bit m_stemDir); le code NULL
	permet un test d'association avec variable "attente" in
	liais_note (attente est engendree par coincidence rel && m_stemLen inter-
	disant le choix de la direction de liaison); ainsi NULL && attente 
	permettent d'entrer dans l'algoritme */ 

	return;	
}				/* fermeture de la fonction */

} // namespace vrv