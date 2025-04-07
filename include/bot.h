/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 20:28:00 by sguzman           #+#    #+#             */
/*   Updated: 2025/04/07 18:18:43 by bautrodr         ###   ########.fr       */
/*   Updated: 2025/04/07 15:44:39 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_H
# define BOT_H

# include "logging.h"
# include "tool.h"
# include <algorithm>
# include <cfloat>
# include <climits>
# include <csignal>
# include <cstdarg>
# include <cstddef>
# include <cstring>
# include <iostream>
# include <map>
# include <netdb.h>
# include <set>
# include <sstream>
# include <string>
# include <vector>

# define READBUFFER_LEN 2048
# define NICKNAME std::string("whisper")
# define USERNAME std::string("whisper")
# define MAX_PARAMS 2
# define PARAMS_MSG 2
# define PARAMS_JOKE 1

class Bot
{
  public:
	static Bot *instance;

	Bot(std::string host, int port, std::string password);
	~Bot(void);

	void Run(void);

  private:
	static void SignalHandler(int sig);
	void Exit(int status);
	void SetSignals(void);
	void InitConnection(int port, std::string &listen_addr);
	void Authenticate(std::string password);
	std::string Read(void);
	void Write(std::string const &msg);

	/*Parsing*/
	void ProcessRequest(std::string &request);
	void Parser(std::string request);
	bool ParseCmd(std::string &request, std::string &command);
	void ParseAction(std::string &request, std::string &request2);
	void ParseParams(std::string &request, std::vector<std::string> &params);
	void ParserAux(std::string &req, std::string &cmd,
		std::vector<std::string> &params);
	void SaveUsers(std::vector<std::string> &users, std::string &ustr);
	std::vector<std::string> userList(std::string users);

	/*Actions*/
	void executeAction(std::string &action, std::string &users,
		std::string &msg, std::string &sender);

	int sock_;
	struct sockaddr_in address_;
};

class Jokes
{
  public:
	static std::string getRandomJoke()
	{
		const char *raw_jokes[] = {
			"How many kids with ADD does it take to change a lightbulb? Let's go ride bikes!",
			"If you want a job in the moisturizer industry, the best advice I can give is to apply daily.",
			"You can't run through a camp site. You can only ran, because it's past tents.",
			"What do birds give out on Halloween? Tweets.",
			"What does a female snake use for support? A co-Bra!",
			"Egyptians claimed to invent the guitar, but they were such lyres.",
			"How do hens stay fit? They always egg-cercise!",
			"Did you hear about the campsite that got visited by Bigfoot? It got in tents.",
			"I can't take my dog to the pond anymore because the ducks keep attacking him. That's what I get for buying a pure bread dog.",
			"Why did the kid cross the playground? To get to the other slide.",
			"I boiled a funny bone last night and had a laughing stock",
			"Why are mummys scared of vacation? They're afraid to unwind.",
			"What's orange and sounds like a parrot? A Carrot.",
			"Why did the cookie cry? Because his mother was a wafer so long",
			"A man tried to sell me a coffin today. I told him that's the last thing I need.",
			"Someone broke into my house last night and stole my limbo trophy. How low can you go?",
			"The great thing about stationery shops is they're always in the same place...",
			"What time did the man go to the dentist? Tooth hurt-y.",
			"What did the Zen Buddist say to the hotdog vendor? Make me one with everything.",
			"The rotation of earth really makes my day.",
			"How do you find Will Smith in the snow?  Look for fresh prints.",
			"Why couldn't the kid see the pirate movie? Because it was rated arrr!",
			"I’ll tell you something about German sausages, they’re the wurst",
			"What did the calculator say to the student? You can count on me.",
			"Why didn’t the skeleton cross the road? Because he had no guts.",
			"The great thing about stationery shops is they're always in the same place...",
			"Why did the coffee file a police report? It got mugged.",
			"How does a penguin build it’s house? Igloos it together.",
			"What's large, grey, and doesn't matter? An irrelephant. ",
			"What do you get if you cross a turkey with a ghost? A poultry-geist!",
			"I went to the doctor today and he told me I had type A blood but it was a type O.",
			"What do you give a sick lemon? Lemonaid.",
			"Did you hear about the runner who was criticized? He just took it in stride",
			"What did the 0 say to the 8? Nice belt.",
			"How do you get two whales in a car? Start in England and drive West.",
			"Why did the fireman wear red, white, and blue suspenders? To hold his pants up.",
			"What’s the longest word in the dictionary? Smiles. Because there’s a mile between the two S’s.",
			"Why are snake races so exciting? They're always neck and neck.",
			"I got fired from a florist, apparently I took too many leaves.",
			"Why do choirs keep buckets handy? So they can carry their tune",
			"What did the traffic light say to the car as it passed? \"Don't look I'm changing!\"",
			"Why did the man put his money in the freezer? He wanted cold hard cash!",
			"A cannibal is someone who is fed up with people.",
			"I thought about going on an all-almond diet. But that's just nuts.",
			"A termite walks into a bar and asks “Is the bar tender here?”",
			"Doctor: Do you want to hear the good news or the bad news? Patient: Good news please. Doctor: we're naming a disease after you.",
			"Where do you take someone who has been injured in a Peek-a-boo accident? To the I.C.U.",
			"I gave my friend 10 puns hoping that one of them would make him laugh. Sadly, no pun in ten did.",
			"My boss told me to attach two pieces of wood together... I totally nailed it!",
			"Where does Fonzie like to go for lunch? Chick-Fil-Eyyyyyyyy.",
			"What did the Zen Buddist say to the hotdog vendor? Make me one with everything.",
			"How do you make holy water? You boil the hell out of it.",
			"What did one nut say as he chased another nut?  I'm a cashew!",
			"What is a tornado's favorite game to play? Twister!",
			"What do you do when you see a space man? Park your car, man.",
			"Why did the m&m go to school? Because it wanted to be a Smartie!",
			"I'll tell you what often gets over looked... garden fences.",
			"What do you call a duck that gets all A's? A wise quacker.",
			"I had a pair of racing snails. I removed their shells to make them more aerodynamic, but they became sluggish.",
			"I wanted to be a tailor but I didn't suit the job",
			"What did the father tomato say to the baby tomato whilst on a family walk? Ketchup.", "Wife: Honey I’m pregnant. Me: Well…. what do we do now? Wife: Well, I guess we should go to a baby doctor. Me: Hm.. I think I’d be a lot more comfortable going to an adult doctor.",
			"I think circles are pointless.",
			"A ghost walks into a bar and asks for a glass of vodka but the bar tender says, “sorry we don’t serve spirits”",
			"I went to the zoo the other day, there was only one dog in it. It was a shitzu.",
			"What is this movie about? It is about 2 hours long.",
		};
		const int JOKES_SIZE = sizeof(raw_jokes) / sizeof(raw_jokes[0]);
		std::vector<std::string> jokes(raw_jokes, raw_jokes + JOKES_SIZE);

		int randomIndex = std::rand() % jokes.size();
		return (jokes[randomIndex]);
	}
};

#endif /* BOT_H */
