#include "Button.h"
#include "Collider.h"


//CONSTRUCTORS
Button::Button(const std::string& text, const size_t& size, const sf::Vector2f& pos,
	const eIndent& indent, const std::string& path, const sf::Texture& texture) : Entity(BUTTON, texture)
{
	std::string	format[3] = { "LEFT", "CENTER", "RIGHT" };

	std::cout << "Add button with text \"" << text << "\"." << std::endl;
	this->_font.loadFromFile(path);
	this->_text.setString(text);
	this->_text.setFont(this->_font);
	this->_text.setCharacterSize(size);
	this->_text.setOrigin(sf::Vector2f(
		(this->_text.getGlobalBounds().width / 2.f) * (float)indent,
		this->_text.getGlobalBounds().height / 2.f));
	std::cout << "Position avant " << format[indent] << " : " << pos.x << " ; " << pos.y << "." << std::endl;
	std::cout << "Position apres " << format[indent] << " : " << pos.x + (pos.x / 2.f) * ((float)indent - 1.f) << " ; " << pos.y << "." << std::endl;
	this->_text.setPosition(pos + sf::Vector2f(
		(pos.x / 2.f) * ((float)indent - 1.f), 0));
	this->_event = NULL;
	this->addCollider(new BoxCollider(this->_text.getGlobalBounds()));
}

Button::Button(const std::string& text, const size_t& size, const sf::Font& font,
	const sf::Vector2f& pos, const eIndent& indent, const sf::Texture& texture) : Entity(BUTTON, texture)
{
	std::cout << "Add button with text \"" << text << "\"." << std::endl;
	this->_font = font;
	this->_text.setString(text);
	this->_text.setFont(this->_font);
	this->_text.setCharacterSize(size);
	this->_text.setOrigin(sf::Vector2f(
		(this->_text.getGlobalBounds().width / 2.f) * (float)indent,
		this->_text.getGlobalBounds().height / 2.f));
	this->_text.setPosition(pos + sf::Vector2f(
		(pos.x / 2.f) * ((float)indent - 1.f), 0));
	this->_event = NULL;

	sf::FloatRect	box = this->_text.getGlobalBounds();

	this->addCollider(new BoxCollider(box));
}

Button::Button(const Button& other) : Entity(other._type, other._texture)
{
	this->_font = sf::Font(other._font);
	this->_text = sf::Text(other._text);
	for (std::vector<BoxCollider *>::const_iterator it = other._colliders.begin(); it != other._colliders.end(); ++it)
		this->_colliders.push_back(new BoxCollider((*it)->getShape().getGlobalBounds()));
	this->_sprite = sf::Sprite(other._sprite);
	this->_event = other._event;
}

Button::~Button()
{
}


//GETTERS
sf::Text&					Button::getText() 
{
	return (this->_text);
}

const std::function<int()>&	Button::getEvent() const
{
	return (this->_event);
}


//SETTERS
void	Button::setEvent(const std::function<int()> event)
{
	this->_event = event;
}


//METHODS
int		Button::triggerEvent()
{
	if (this->_event != NULL)
		return(this->_event());
}