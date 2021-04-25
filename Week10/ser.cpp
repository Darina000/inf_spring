
#include <iostream>
#include <boost/asio.hpp>

void read_information_while(boost::asio::ip::tcp::socket& socket);
void write_information(boost::asio::ip::tcp::socket& socket, const std::string& user_name);

int main()
{
    const std::size_t size = 33;

    auto port = 7777;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    boost::asio::io_service io_service;

    std::string user_name = "Happy server";

    try
    {
        boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        boost::asio::ip::tcp::socket socket(io_service);

        acceptor.accept(socket);
        auto reader = std::thread(read_information_while, std::ref(socket));
        write_information(socket, user_name);
        reader.join();

    }
    catch (boost::system::system_error& error)
    {
        std::cout << "Error occured! Error code = " << error.code() << ". Message: " << error.what() << "\n";

        system("pause");

        return error.code().value();
    }

    return 0;
}

void read_information_while(boost::asio::ip::tcp::socket& socket)
{
    boost::asio::streambuf buffer;
    std::string message;

    do
    {
        boost::asio::read_until(socket, buffer, '\n');

        std::istream input_stream(&buffer);
        std::getline(input_stream, message, '\n');

        std::cout << message << std::endl;
    } while (message != "The conversation is ended");

}

void write_information(boost::asio::ip::tcp::socket& socket, const std::string& user_name)
{
    boost::asio::write(socket, boost::asio::buffer("Now Happy server is ready\n"));

    std::string message;

    std::getline(std::cin, message);
    while (message != "exit")
    {
        boost::asio::write(socket, boost::asio::buffer(user_name + " : " + message + "\n"));
        std::getline(std::cin, message);
    }

    message = "The conversation is ended\n";

    boost::asio::write(socket, boost::asio::buffer(message));

}
