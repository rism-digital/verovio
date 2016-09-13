Writing an importer
-------------------

Verovio supports import for Plain and Easy and for Darms codes (See the PaeInput and DarmsInput classes). Writing an importer from another code could follow the same approach. The code below illustrates how the following snippet can be created in Verovio.

![Snippet](https://cloud.githubusercontent.com/assets/3487289/9319255/2f5187de-4501-11e5-808b-897ed35aa6dd.png)

<<<<<<< HEAD
Some features (articulation, endings, etc.) are still missing but will be added eventually. The resulting MEI file is available [here](importer.mei) and the SVG output for version 0.9.9 [there](importer.svg).
=======
Some features (articulation, endings, etc.) are still missing but will be added eventually. The resulting MEI file is available [here](importer.mei) and the SVG output for version 0.9.12-dev-0727ade [there](importer.svg).
>>>>>>> develop

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
<<<<<<< HEAD
=======
Ending *ending = NULL;
>>>>>>> develop

// start and end id
std::string startid;
std::string endid;

////////////////////////////////////
<<<<<<< HEAD
// Score definition
=======
// The score as score-based MEI
////////////////////////////////////

m_doc->Reset();
Score *score = m_doc->CreateScoreBuffer();
// the section
Section *section = new Section();
score->AddChild(section);

////////////////////////////////////
// ScoreDef definition
>>>>>>> develop
////////////////////////////////////

// staffGrp and staffDef
StaffGrp *staffGrp = new StaffGrp();
StaffDef *staffDef = new StaffDef();
<<<<<<< HEAD
staffDef->SetN( 1 );
staffDef->SetLines(5);

// clef
staffDef->SetClefLine(2);
staffDef->SetClefShape(CLEFSHAPE_G);

// key and meter signatures can be put at in the doc scoreDef
m_doc->m_scoreDef.SetKeySig(KEYSIGNATURE_2s);
m_doc->m_scoreDef.SetMeterSym(METERSIGN_common);

// adding the staffDef to the staffGrp and the staffGrp to the doc scoreDef
staffGrp->AddStaffDef(staffDef);
m_doc->m_scoreDef.AddStaffGrp(staffGrp);

// we need one page and one system
Page *page = new Page();
m_doc->AddPage(page);
System *system = new System();
page->AddSystem(system);
=======
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
>>>>>>> develop

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
<<<<<<< HEAD
layer->AddLayerElement(rest);
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
layer->AddChild(rest);
staff->AddChild(layer);
measure->AddChild(staff);
section->AddChild(measure);
>>>>>>> develop

////////////////////////////////////
// Multi-rest measure
////////////////////////////////////

<<<<<<< HEAD
// create another measure and content
=======
>>>>>>> develop
measure = new Measure();
measure->SetN(1);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
<<<<<<< HEAD

// add a multi-rest
=======
>>>>>>> develop
multiRest = new MultiRest();
multiRest->SetNum(3);

// add them up
<<<<<<< HEAD
layer->AddLayerElement(multiRest);
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
layer->AddChild(multiRest);
staff->AddChild(layer);
measure->AddChild(staff);
section->AddChild(measure);
>>>>>>> develop

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
<<<<<<< HEAD
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_c);
note->SetOct(5);
beam->AddLayerElement(note);
=======
beam->AddChild(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
<<<<<<< HEAD
beam->AddLayerElement(note);
tuplet->AddLayerElement(beam);
layer->AddLayerElement(tuplet);
=======
beam->AddChild(note);
tuplet->AddChild(beam);
layer->AddChild(tuplet);
>>>>>>> develop

tuplet = new Tuplet;
tuplet->SetNum(3);
tuplet->SetNumbase(2);
beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
<<<<<<< HEAD
note->SetOct(5);
beam->AddLayerElement(note);
=======
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
<<<<<<< HEAD

// keep the id as start id for the tie
startid = note->GetUuid();
beam->AddLayerElement(note);
tuplet->AddLayerElement(beam);
layer->AddLayerElement(tuplet);
=======
// keep the id as start id for the tie
startid = note->GetUuid();
beam->AddChild(note);
tuplet->AddChild(beam);
layer->AddChild(tuplet);
>>>>>>> develop

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_g);
<<<<<<< HEAD
note->SetOct(5);

// keep the id as end id for the tie
endid = note->GetUuid();
layer->AddLayerElement(note);
=======
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
// keep the id as end id for the tie
endid = note->GetUuid();
layer->AddChild(note);
>>>>>>> develop

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
<<<<<<< HEAD
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetOct(5);
chord->AddLayerElement(note);
beam->AddLayerElement(chord);
=======
chord->AddChild(note);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
chord->AddChild(note);
beam->AddChild(chord);
>>>>>>> develop
chord = new Chord();
chord->SetDur(DURATION_8);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
<<<<<<< HEAD
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_b);
note->SetOct(4);
chord->AddLayerElement(note);
beam->AddLayerElement(chord);
layer->AddLayerElement(beam);

// add them up
staff->AddLayer(layer);
measure->AddStaff(staff);

// add the tie element to the measure
measure->AddFloatingElement(tie);
system->AddMeasure(measure);
=======
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
>>>>>>> develop

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
<<<<<<< HEAD
layer->AddLayerElement(note);
=======
layer->AddChild(note);
>>>>>>> develop

chord = new Chord();
chord->SetDur(DURATION_breve);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
endid = note->GetUuid();
<<<<<<< HEAD
chord->AddLayerElement(note);
=======
chord->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetPname(PITCHNAME_d);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
<<<<<<< HEAD
chord->AddLayerElement(note);
layer->AddLayerElement(chord);

// create the slur
slur = new Slur();
slur->SetStartid(startid);
slur->SetEndid(endid);

// add the slur to the previous measure
dynamic_cast<Measure*>(system->m_children.back())->AddFloatingElement(slur);

// add the content
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
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
>>>>>>> develop

////////////////////////////////////
// Changing the scoreDef
////////////////////////////////////

scoreDef = new ScoreDef();
scoreDef->SetMeterCount(6);
scoreDef->SetMeterUnit(8);
scoreDef->SetKeySig(KEYSIGNATURE_0);

<<<<<<< HEAD
system->AddScoreDef(scoreDef);
=======
section->AddChild(scoreDef);
>>>>>>> develop

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
<<<<<<< HEAD
layer->AddLayerElement(note);
=======
layer->AddChild(note);
>>>>>>> develop

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetOct(5);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
layer->AddLayerElement(beam);
=======
beam->AddChild(note);
layer->AddChild(beam);
>>>>>>> develop

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetOct(5);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);
=======
beam->AddChild(note);
layer2->AddChild(beam);
>>>>>>> develop

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
note->SetOct(4);
<<<<<<< HEAD
layer2->AddLayerElement(note);

// add them up
staff->AddLayer(layer);
staff->AddLayer(layer2);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
layer2->AddChild(note);

// add them up
staff->AddChild(layer);
staff->AddChild(layer2);
measure->AddChild(staff);
section->AddChild(measure);
>>>>>>> develop

////////////////////////////////////
// Seventh measure
////////////////////////////////////

<<<<<<< HEAD
=======
ending = new Ending();
ending->SetN(1);
>>>>>>> develop
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
note->SetOct(5);
<<<<<<< HEAD
layer->AddLayerElement(note);
=======
layer->AddChild(note);
>>>>>>> develop

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
<<<<<<< HEAD
layer->AddLayerElement(note);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddLayerElement(note);
=======
layer->AddChild(note);

beam = new Beam();
note = new Note();
startid = note->GetUuid();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);
=======
beam->AddChild(note);
layer2->AddChild(beam);
>>>>>>> develop

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_f);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
=======
beam->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
note = new Note();
=======
beam->AddChild(note);
note = new Note();
endid = note->GetUuid();
>>>>>>> develop
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
note->SetOct(4);
<<<<<<< HEAD
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);

// add them up
staff->AddLayer(layer);
staff->AddLayer(layer2);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
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
>>>>>>> develop

////////////////////////////////////
// Eighth measure
////////////////////////////////////

<<<<<<< HEAD
=======
ending = new Ending();
ending->SetN(2);
>>>>>>> develop
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
note->SetOct(5);
<<<<<<< HEAD
chord->AddLayerElement(note);
=======
chord->AddChild(note);
>>>>>>> develop
note = new Note();
note->SetPname(PITCHNAME_e);
note->SetOct(4);
note->SetFermata(PLACE_below);
<<<<<<< HEAD
chord->AddLayerElement(note);
layer->AddLayerElement(chord);

// add them up
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);
=======
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
>>>>>>> develop
```

