Writing an importer
-------------------

Verovio supports import for Plain and Easy and for Darms codes (See the PaeInput and DarmsInput classes). Writing an importer from another code could follow the same approach. The code below illustrates how the following snippet can be created in Verovio.

![Snippet](https://cloud.githubusercontent.com/assets/3487289/9319255/2f5187de-4501-11e5-808b-897ed35aa6dd.png)

Some features (articulation, endings, etc.) are still missing but will be added eventually. The resulting MEI file is available [here](importer.mei) and the SVG output for version 0.9.12-dev-0727ade [there](importer.svg).

``` C++
////////////////////////////////////
// The document
////////////////////////////////////

Doc *doc = new Doc();

////////////////////////////////////
// Some variables
////////////////////////////////////

Measure *measure = NULL;
Staff *staff = NULL;
Layer *layer = NULL;
Layer *layer2 = NULL;
Note *note = NULL;
Beam *beam = NULL;
Tuplet *tuplet = NULL;
Rest *rest = NULL;
MultiRest *multiRest = NULL;
Tie *tie = NULL;
Chord *chord = NULL;
Slur *slur = NULL;
ScoreDef *scoreDef = NULL;
Ending *ending = NULL;

// start and end id
std::string startid;
std::string endid;

////////////////////////////////////
// The score as score-based MEI
////////////////////////////////////

m_doc->Reset();
Score *score = m_doc->CreateScoreBuffer();
// the section
Section *section = new Section();
score->AddChild(section);

////////////////////////////////////
// ScoreDef definition
////////////////////////////////////

// staffGrp and staffDef
StaffGrp *staffGrp = new StaffGrp();
StaffDef *staffDef = new StaffDef();
staffDef->SetN(1);
staffDef->SetLines(5);
// clef
staffDef->SetClefLine(2);
staffDef->SetClefShape(CLEFSHAPE_G);
// key and meter signatures can be put at in the scoreDef
m_doc->m_scoreDef.SetKeySig(KEYSIGNATURE_2s);
m_doc->m_scoreDef.SetMeterSym(METERSIGN_common);
// adding the staffDef to the staffGrp and the staffGrp to the doc scoreDef
staffGrp->AddChild(staffDef);
m_doc->m_scoreDef.AddChild(staffGrp);

////////////////////////////////////
// Up-beat measure
////////////////////////////////////

// create the measure and its content
measure = new Measure();
measure->SetN(0);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
rest = new Rest();
rest->SetDur(DURATION_8);

// add them up
layer->AddChild(rest);
staff->AddChild(layer);
measure->AddChild(staff);
section->AddChild(measure);

////////////////////////////////////
// Multi-rest measure
////////////////////////////////////

measure = new Measure();
measure->SetN(1);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
multiRest = new MultiRest();
multiRest->SetNum(3);

// add them up
layer->AddChild(multiRest);
staff->AddChild(layer);
measure->AddChild(staff);
section->AddChild(measure);

////////////////////////////////////
// Fourth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(4);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

tuplet = new Tuplet;
tuplet->SetNum(3);
tuplet->SetNumbase(2);
beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
beam->AddChild(note);
tuplet->AddChild(beam);
layer->AddChild(tuplet);

tuplet = new Tuplet;
tuplet->SetNum(3);
tuplet->SetNumbase(2);
beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
// keep the id as start id for the tie
startid = note->GetUuid();
beam->AddChild(note);
tuplet->AddChild(beam);
layer->AddChild(tuplet);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_g);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
// keep the id as end id for the tie
endid = note->GetUuid();
layer->AddChild(note);

// create the tie with appropriate start and end ids
tie = new Tie();
tie->SetStartid(startid);
tie->SetEndid(endid);

beam = new Beam();
chord = new Chord();
chord->SetDur(DURATION_8);
note = new Note();
note->SetPname(PITCHNAME_a);
note->SetOct(5);
startid = note->GetUuid();
chord->AddChild(note);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
chord->AddChild(note);
beam->AddChild(chord);
chord = new Chord();
chord->SetDur(DURATION_8);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
chord->AddChild(note);
note = new Note();
note->SetPname(PITCHNAME_b);
note->SetOct(4);
chord->AddChild(note);
beam->AddChild(chord);
layer->AddChild(beam);

// add the hairpin
Hairpin *hairpin = new Hairpin();
hairpin->SetForm(hairpinLog_FORM_dim);
hairpin->AddStaff(1);
hairpin->SetTstamp(2.8);
hairpin->SetTstamp2(std::make_pair(1, 0.9));
// add the slur to the measure
measure->AddChild(hairpin);

// add them up
staff->AddChild(layer);
measure->AddChild(staff);
// add the tie element to the measure
measure->AddChild(tie);
section->AddChild(measure);

////////////////////////////////////
// Fifth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(5);
measure->SetRight(BARRENDITION_rptstart);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

note = new Note();
note->SetGrace(GRACE_acc);
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
note->SetStemDir(STEMDIRECTION_up);
layer->AddChild(note);

chord = new Chord();
chord->SetDur(DURATION_breve);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
endid = note->GetUuid();
chord->AddChild(note);
note = new Note();
note->SetPname(PITCHNAME_d);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
chord->AddChild(note);
layer->AddChild(chord);

slur = new Slur();
slur->SetStartid(startid);
slur->SetEndid(endid);
// add the slur to the measure
section->GetLast()->AddChild(slur);

// add the hairpin
Dynam *dynam = new Dynam();
dynam->AddStaff(1);
dynam->SetTstamp(1.0);
Text *text = new Text();
text->SetText(L"p");
dynam->AddChild(text);
// add the slur to the measure
measure->AddChild(dynam);

// add them up
staff->AddChild(layer);
measure->AddChild(staff);
section->AddChild(measure);

////////////////////////////////////
// Changing the scoreDef
////////////////////////////////////

scoreDef = new ScoreDef();
scoreDef->SetMeterCount(6);
scoreDef->SetMeterUnit(8);
scoreDef->SetKeySig(KEYSIGNATURE_0);

section->AddChild(scoreDef);

////////////////////////////////////
// Sixth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(6);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
layer2 = new Layer();
layer2->SetN(2);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
layer->AddChild(note);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddChild(note);
layer->AddChild(beam);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddChild(note);
layer2->AddChild(beam);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(4);
layer2->AddChild(note);

// add them up
staff->AddChild(layer);
staff->AddChild(layer2);
measure->AddChild(staff);
section->AddChild(measure);

////////////////////////////////////
// Seventh measure
////////////////////////////////////

ending = new Ending();
ending->SetN(1);
measure = new Measure();
measure->SetN(7);
measure->SetRight(BARRENDITION_rptend);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
layer2 = new Layer();
layer2->SetN(2);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
layer->AddChild(note);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
layer->AddChild(note);

beam = new Beam();
note = new Note();
startid = note->GetUuid();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(4);
beam->AddChild(note);
layer2->AddChild(beam);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_f);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddChild(note);
note = new Note();
endid = note->GetUuid();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
note->SetOct(4);
beam->AddChild(note);
layer2->AddChild(beam);

slur = new Slur();
slur->SetStartid(startid);
slur->SetEndid(endid);
// add the slur to the measure
section->GetLast()->AddChild(slur);

// add them up
staff->AddChild(layer);
staff->AddChild(layer2);
measure->AddChild(staff);
ending->AddChild(measure);
section->AddChild(ending);

////////////////////////////////////
// Eighth measure
////////////////////////////////////

ending = new Ending();
ending->SetN(2);
measure = new Measure();
measure->SetN(8);
measure->SetRight(BARRENDITION_end);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

chord = new Chord();
chord->SetDur(DURATION_4);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
chord->AddChild(note);
note = new Note();
note->SetPname(PITCHNAME_e);
note->SetOct(4);
note->SetFermata(PLACE_below);
chord->AddChild(note);
layer->AddChild(chord);

// add them up
staff->AddChild(layer);
measure->AddChild(staff);
ending->AddChild(measure);
section->AddChild(ending);

////////////////////////////////////
// Convert the Doc to page-based MEI
////////////////////////////////////

m_doc->ConvertToPageBasedDoc();
```

