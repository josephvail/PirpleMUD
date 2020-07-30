/*!
 * @file PirpleMUD.h
 * @brief Main PirpleMUD header to include when running the game.
 * @details All of the headers necessary to run the game.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa pirple_.h, pirple_utils_.h, CMUD.h
 */
#ifndef PIRPLEMUD_PIRPLEMUD_H
#define PIRPLEMUD_PIRPLEMUD_H

#include "CMUD.h"

/*!
 * @namespace Pirple
 * @brief The top-level scope for all PirpleMUD objects.
 */
namespace Pirple
{
}

/*!
 * @mainpage PirpleMUD Coding Documentation
 * This is documentation for my implementation of CircleMUD in C++.
 * I take no credit for previous authors. They were the reason I started
 * programming. I thank them for their work. This is a hobby for me, and
 * something I have been interested in doing for a while. If I miss any
 * credits, or seem to move outside the boundaries of the license, let
 * me know.
 *
 * Please browse the documentation under the Pirple namespace, and the
 * references to CircleMUD under it's namespace. If I make any references
 * that use the wrong definition, or you feel you have a better definition,
 * let me know.
 *
 * If you have any questions, comments, or concerns you can e-mail me
 * and I will do my best to respond.
 * @n
 * @n-Joe Vail
 */


/*!
 * @page LICENSE LICENSE
 * @tableofcontents
 * @section pirple_license PirpleMUD License
 * Copyright (c) 2018 by jvindy software. All Rights Reserved.@n
 * PirpleMUD was created by:@n
 * Joseph Vail@n
 * jvindy software@n
 * Caro, MI 48723 USA@n
 * joseph.vail@gmail.com@n
 * @n
 * @n
 * @n
 * @n
 *
 * PirpleMUD is licensed software.  This file contains the text of the PirpleMUD
 * license.  If you wish to use the PirpleMUD system in any way, or use any of
 * its source code, you must read this license and are legally bound to comply
 * with it.
 *
 * PirpleMUD is a derivative work based on the CircleMUD system written by Jeremy
 * Elson.  CircleMUD is also licensed software; you are legally bound to comply
 * with the original CircleMUD license, the DikuMUD license, as well as the
 * PirpleMUD license if you wish to use PirpleMUD.
 *
 * Use of the PirpleMUD code in any capacity implies that you have read,
 * understood, and agreed to abide by the terms and conditions set down by this
 * license, the CircleMUD License, and the DikuMUD license.  If you use
 * PirpleMUD without complying with the license, you are breaking the law.
 *
 * Using PirpleMUD legally is easy.  In short, the license requires four things:
 *
 * 1) You must not use PirpleMUD to make money or be compensated in any way.@n
 * 2) You must give the authors credit for their work.@n
 * 3) You must comply with the CircleMUD license.@n
 * 4) You must comply with the DikuMUD license.@n
 * @n
 * @subsection pp1 First Condition
 * The first condition says that you must not use PirpleMUD to make money in
 * any way or be otherwise compensated.  PirpleMUD was developed in people's
 * uncompensated spare time and was given to you free of charge, and you must
 * not use it to make money.  PirpleMUD must not in any way be used to
 * facilitate your acceptance of fees, donations, or other compensation.
 * Examples include, but are not limited to the following:
 *
 * If you run PirpleMUD, you must not require any type of fee or donation
 * in exchange for being able to play PirpleMUD.  You must not solicit,
 * offer or accept any kind of donation from your players in exchange for
 * enhanced status in the game such as increased levels, character stats,
 * gold, or equipment.
 *
 * You must not solicit or accept money or other donations in exchange for
 * running PirpleMUD.  You must not accept money or other donations from
 * your players for purposes such as hardware upgrades for running
 * PirpleMUD.
 *
 * You must not sell PirpleMUD.  You must not accept any type of fee in
 * exchange for distributing or copying PirpleMUD.
 *
 * If you are a PirpleMUD administrator, You must not accept any type of
 * reimbursement for money spent out of pocket for running PirpleMUD, i.e.,
 * for equipment expenses or fees incurred from service providers.
 *
 * @subsection pp2 Second Condition
 * The second part of the license states that you must give credit to the
 * creators of PirpleMUD.  A great deal of work went into the creation of
 * PirpleMUD, and it was given to you completely free of charge; claiming that
 * you wrote the MUD yourself is a slap in the face to everyone who worked to
 * bring you a high quality product while asking for nothing but credit for
 * their work in return.
 *
 * Specifically, the following are required:
 *
 * The text in the 'credits' file distributed with PirpleMUD must be
 * preserved.  You may add your own credits to the file, but the existing
 * text must not be removed, abridged, truncated, or changed in any way.
 * This file must be displayed when the 'credits' command is used from
 * within the MUD.
 *
 * The "PIRPLEMUD" help entry must be maintained intact and unchanged, and
 * displayed in its entirety when the 'help pirplemud' command is used.
 *
 * The login sequence must contain the names of the DikuMUD, CircleMUD and
 * PirpleMUD creators.  The 'login sequence' is defined as the text seen by
 * players between the time they connect to the MUD and when they start to
 * play the game itself.
 *
 * This license must be distributed AS IS with all copies or portions of
 * the PirpleMUD that you distribute, if any, including works derived from
 * PirpleMUD.
 *
 * You must not remove, change, or modify any notices of copyright,
 * licensing or authorship found in any PirpleMUD source code files.
 *
 * Claims that any of the above requirements are inapplicable to a particular
 * MUD for reasons such as "our MUD is totally rewritten" or similar are
 * completely invalid.  If you can write a MUD completely from scratch then
 * you are encouraged to do so by all means, but use of any part of the
 * PirpleMUD, CircleMUD, or DikuMUD source code requires that their
 * respective licenses be followed, including the crediting requirements.
 *
 * @subsection pp3 Third Condition
 * The third part of the license states that you must comply with the
 * CircleMUD license.  This is required because PirpleMUD is a CircleMUD
 * derivative. The CircleMUD license is included below.
 *
 * @subsection pp4 Fourth Condition
 * The fourth part of the license states that you must comply with the
 * DikuMUD license.  This is required because PirpleMUD is a DikuMUD
 * derivative. The DikuMUD license is included below.
 *
 * @subsection pp5 Summary
 * You are allowed to use, modify and redistribute all PirpleMUD source code
 * and documentation as long as such use does not violate any of the rules
 * set down by this license.
 *
 * -Joe Vail
 *
 * PirpleMUD -- Copyright (c) 2018, jvindy software@n
 * Other portions copyright by authors as noted in ChangeLog and source code.
 *
 * @section circle_license CircleMUD License
 * Copyright (C) 1995 by Jeremy Elson. All Rights Reserved.@n
 * CircleMUD was created by:@n
 * Jeremy Elson@n
 * Department of Computer Science@n
 * Johns Hopkins University@n
 * Baltimore, MD  21218  USA@n
 * jelson@circlemud.org@n
 * @n
 * @n
 * @n
 * @n
 * CircleMUD is licensed software.  This file contains the text of the CircleMUD
 * license.  If you wish to use the CircleMUD system in any way, or use any of
 * its source code, you must read this license and are legally bound to comply
 * with it.
 *
 * CircleMUD is a derivative work based on the DikuMUD system written by Hans
 * Henrik Staerfeldt, Katja Nyboe, Tom Madsen, Michael Seifert, and Sebastian
 * Hammer.  DikuMUD is also licensed software; you are legally bound to comply
 * with the original DikuMUD license as well as the CircleMUD license if you
 * wish to use CircleMUD.
 *
 * Use of the CircleMUD code in any capacity implies that you have read,
 * understood, and agreed to abide by the terms and conditions set down by this
 * license and the DikuMUD license.  If you use CircleMUD without complying with
 * the license, you are breaking the law.
 *
 * Using CircleMUD legally is easy.  In short, the license requires three things:
 *
 * 1) You must not use CircleMUD to make money or be compensated in any way.@n
 * 2) You must give the authors credit for their work.@n
 * 3) You must comply with the DikuMUD license.@n
 * @n
 * That's it -- those are the main conditions set down by this license.
 * Unfortunately, past experience has shown that many people are not willing
 * to follow the spirit of the license, so the remainder of this document will
 * clearly define those conditions in an attempt to prevent people from
 * circumventing them.
 *
 * @subsection cc1 First Condition
 * The first condition says that you must not use CircleMUD to make money in
 * any way or be otherwise compensated.  CircleMUD was developed in people's
 * uncompensated spare time and was given to you free of charge, and you must
 * not use it to make money.  CircleMUD must not in any way be used to
 * facilitate your acceptance of fees, donations, or other compensation.
 * Examples include, but are not limited to the following:
 *
 * If you run CircleMUD, you must not require any type of fee or donation
 * in exchange for being able to play CircleMUD.  You must not solicit,
 * offer or accept any kind of donation from your players in exchange for
 * enhanced status in the game such as increased levels, character stats,
 * gold, or equipment.
 *
 * You must not solicit or accept money or other donations in exchange for
 * running CircleMUD.  You must not accept money or other donations from
 * your players for purposes such as hardware upgrades for running
 * CircleMUD.
 *
 * You must not sell CircleMUD.  You must not accept any type of fee in
 * exchange for distributing or copying CircleMUD.
 *
 * If you are a CircleMUD administrator, You must not accept any type of
 * reimbursement for money spent out of pocket for running CircleMUD, i.e.,
 * for equipment expenses or fees incurred from service providers.
 *
 * @subsection cc2 Second Condition
 * The second part of the license states that you must give credit to the
 * creators of CircleMUD.  A great deal of work went into the creation of
 * CircleMUD, and it was given to you completely free of charge; claiming that
 * you wrote the MUD yourself is a slap in the face to everyone who worked to
 * bring you a high quality product while asking for nothing but credit for
 * their work in return.
 *
 * Specifically, the following are required:
 *
 * The text in the 'credits' file distributed with CircleMUD must be
 * preserved.  You may add your own credits to the file, but the existing
 * text must not be removed, abridged, truncated, or changed in any way.
 * This file must be displayed when the 'credits' command is used from
 * within the MUD.
 *
 * The "CIRCLEMUD" help entry must be maintained intact and unchanged, and
 * displayed in its entirety when the 'help circlemud' command is used.
 *
 * The login sequence must contain the names of the DikuMUD and CircleMUD
 * creators.  The 'login sequence' is defined as the text seen by players
 * between the time they connect to the MUD and when they start to play
 * the game itself.
 *
 * This license must be distributed AS IS with all copies or portions of
 * the CircleMUD that you distribute, if any, including works derived from
 * CircleMUD.
 *
 * You must not remove, change, or modify any notices of copyright,
 * licensing or authorship found in any CircleMUD source code files.
 *
 * Claims that any of the above requirements are inapplicable to a particular
 * MUD for reasons such as "our MUD is totally rewritten" or similar are
 * completely invalid.  If you can write a MUD completely from scratch then
 * you are encouraged to do so by all means, but use of any part of the
 * CircleMUD or DikuMUD source code requires that their respective licenses
 * be followed, including the crediting requirements.
 *
 * @subsection cc3 Third Condition
 * The third part of the license simply states that you must comply with the
 * DikuMUD license.  This is required because CircleMUD is a DikuMUD derivative.
 * The DikuMUD license is included below.
 *
 * @subsection cc4 Summary
 *
 * You are allowed to use, modify and redistribute all CircleMUD source code
 * and documentation as long as such use does not violate any of the rules
 * set down by this license.
 *
 * -Jeremy Elson
 *
 * CircleMUD 3 -- Copyright (C) 1994-2001, The CircleMUD Group.@n
 * Other portions copyright by authors as noted in ChangeLog and source code.
 * @n
 * @section diku_license DikuMUD License
 * Copyright (C) 1990, 1991. All Rights Reserved.@n
 *                       Program & Concept created by@n
 * @n
 * Sebastian Hammer@n
 * Prss. Maries Alle 15, 1@n
 * 1908 Frb. C.@n
 * DENMARK@n
 * (email quinn@freja.diku.dk)@n
 * @n
 * Michael Seifert@n
 * Nr. Soeg. 37C, 1, doer 3@n
 * 1370 Copenhagen K.@n
 * DENMARK@n
 * (email seifert@freja.diku.dk)@n
 * @n
 * Hans Henrik St{rfeldt@n
 * Langs} 19@n
 * 3500 V{rl|se@n
 * DENMARK@n
 * (email bombman@freja.diku.dk)@n
 * @n
 * Tom Madsen@n
 * R|de Mellemvej 94B, 64@n
 * 2300 Copenhagen S.@n
 * DENMARK@n
 * (email noop@freja.diku.dk)@n
 * @n
 * Katja Nyboe@n
 * Kildeg}rdsvej 2@n
 * 2900 Hellerup@n
 * 31 62 82 84@n
 * DENMARK@n
 * (email katz@freja.diku.dk)@n
 * @n
 * This document contains the rules by which you can use, alter or publish
 * parts of DikuMud. DikuMud has been created by the above five listed persons
 * in their spare time, at DIKU (Computer Science Instutute at Copenhagen
 * University). You are legally bound to follow the rules described in this
 * document.
 *
 * Rules:
 *
 * !! DikuMud is NOT Public Domain, shareware, careware or the like !!
 *
 * You may under no circumstances make profit on *ANY* part of DikuMud in
 * any possible way. You may under no circumstances charge money for
 * distributing any part of dikumud - this includes the usual $5 charge
 * for "sending the disk" or "just for the disk" etc.
 * By breaking these rules you violate the agreement between us and the
 * University, and hence will be sued.
 *
 * You may not remove any copyright notices from any of the documents or
 * sources given to you.
 *
 * This license must *always* be included "as is" if you copy or give
 * away any part of DikuMud (which is to be done as described in this
 * document).
 *
 * If you publish *any* part of dikumud, we as creators must appear in the
 * article, and the article must be clearly copyrighted subject to this
 * license. Before publishing you must first send us a message, by
 * snail-mail or e-mail, and inform us what, where and when you are
 * publishing (remember to include your address, name etc.)
 *
 * If you wish to setup a version of DikuMud on any computer system, you
 * must send us a message , by snail-mail or e-mail, and inform us where
 * and when you are running the game. (remember to include
 * your address, name etc.)
 *
 * Any running version of DikuMud must include our names in the login
 * sequence. Furthermore the "credits" command shall always cointain
 * our name, addresses, and a notice which states we have created DikuMud.
 *
 * You are allowed to alter DikuMud, source and documentation as long as
 * you do not violate any of the above stated rules.
 *
 *
 * Regards,
 *
 * The DikuMud Group
 *
 * Note:
 *
 * We hope you will enjoy DikuMud, and encourage you to send us any reports
 * on bugs (when you find 'it'). Remember that we are all using our spare
 * time to write and improve DikuMud, bugs, etc. - and changes will take their
 * time. We have so far put extremely many programming hours into this project.
 * If you make any major improvements on DikuMud we would be happy to
 * hear from you. As you will naturally honor the above rules, you will receive
 * new updates and improvements made to the game.
 */

/*!
 * @page contact Contact PirpleMUD
 * @tableofcontents
 * The contact page for PirpleMUD. So far, just me.
 * @section head_coder Head Coder
 * @subsection joe_vail Joe Vail
 * @subsubsection email e-mail
 * joseph.vail@gmail.com
 * @subsubsection jv_resume resume
 * stuff
 */
#endif //PIRPLEMUD_PIRPLEMUD_H
