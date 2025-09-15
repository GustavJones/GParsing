#pragma once
#include "GParsing/HTML/HTMLElement.hpp"
#include "GParsing/HTML/HTMLDefs.hpp"
#include <string>

namespace GParsing {
	struct HTMLDoctype
	{
	private:
		enum class State
		{
			Root,
			Publicity,
			Declaration
		};

	public:
		std::string root;
		std::string publicity;
		std::string declaration;
		
		template <typename CharT>
		void Parse(const GParsing::HTMLToken<CharT>& _element) {
			if (_element.Type() != HTMLTokenType::DOCTYPE)
			{
				throw std::runtime_error("Not a doctype token");
			}

			root = "";
			publicity = "";
			declaration = "";

			State state = State::Root;
			size_t index = 0;

			while (index < _element.Size())
			{
				const char c = _element.GetCharacter(index);

				switch (state)
				{
				case GParsing::HTMLDoctype::State::Root:
					if (IsWhitespace(c))
					{
						state = State::Publicity;

						while (index < _element.Size() && IsWhitespace(_element.GetCharacter(index)))
						{
							index++;
						}

						index--;

						break;
					}
					else
					{
						root += c;
					}

					break;
				case GParsing::HTMLDoctype::State::Publicity:
					if (IsWhitespace(c))
					{
						state = State::Declaration;

						while (index < _element.Size() && IsWhitespace(_element.GetCharacter(index)))
						{
							index++;
						}

						index--;

						break;
					}
					else
					{
						publicity += c;
					}

					break;
				case GParsing::HTMLDoctype::State::Declaration:
					if (IsWhitespace(c))
					{
						break;
					}

					declaration += c;
					break;
				}

				index++;
			}
		}

		bool IsValid() const {
			if (root != "")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
}
