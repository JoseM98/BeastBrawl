#include "UDPClient.h"
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <deque>
#include "../Systems/Utils.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace boost::asio;
using namespace std::chrono;
using namespace std;

UDPClient::UDPClient(string host_, string port_)
    : context{},
      serverEndpoint{*udp::resolver(context).resolve(udp::v4(), host_, port_).begin()},
      socket(context),
      butler{[&]() {
          boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard(context.get_executor());
          context.run();
          cout << "///////////////////////// SALIMOS DEL CONTEXT!!! ///////////////////////////////" << endl;
      }} {
    cout << "Server endpoint is " << serverEndpoint.address() << ":" << serverEndpoint.port() << endl;
    socket.open(udp::v4());
    // udp::endpoint localEndpoint = socket.local_endpoint();
    // cout << "Local endpoint is " << socket.local_endpoint().address() << ":" << socket.local_endpoint().port() << endl;

    StartReceiving();
}

void UDPClient::StartReceiving() {
    // udp::endpoint senderEndpoint;
    cout << "Esperamos recibir datos" << endl;
    socket.async_receive_from(
        asio::buffer(recvBuff),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleReceived,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::SendDateTime() {
    // cout << "Vamos a enviar datos" << endl;
    boost::shared_ptr<string> message(new string(GetTime()));
    socket.async_send_to(
        boost::asio::buffer(*message),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentDateTime,
            this,
            message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::SendInput(Constants::InputTypes newInput) {
    // boost::array<uint16_t, 2> aux{Constants::PetitionTypes::SEND_INPUT, newInput};
    // boost::array<uint16_t,1> sendInput {newInput};
    // sendBuff.push_back(boost::asio::buffer(callType));
    // sendBuff.push_back(boost::asio::buffer(sendInput));
    // Constants::PetitionTypes callType = Constants::PetitionTypes::SEND_INPUT;
    // std::shared_ptr<Constants::InputTypes> auxInput = make_shared<Constants::InputTypes>(newInput);
    
    Constants::PetitionTypes callType = Constants::PetitionTypes::SEND_INPUT;
    sendBuff.clear();
    sendBuff.push_back(boost::asio::buffer(&callType, sizeof(callType)));
    sendBuff.push_back(boost::asio::buffer(&newInput, sizeof(newInput)));
    // boost::array<Constants::InputTypes, 1> input = {newInput};
    // boost::array<mutable_buffer, 2> outputBuffer = {
    //     // boost::asio::buffer(petitionType),
    //     petitionType,
    //     boost::asio::buffer(ojete)};
    
    socket.async_send_to(
        // boost::asio::buffer(outputBuffer),
        boost::asio::buffer(sendBuff),
        serverEndpoint,
        // boost::bind(
        //     &UDPClient::HandleSentInput,
        //     this,
        //     newInput,
        //     boost::asio::placeholders::error,
        //     boost::asio::placeholders::bytes_transferred)
        [&](const boost::system::error_code& errorCode,
            std::size_t bytes_transferred) {
            HandleSentInput(Constants::InputTypes::BACK, errorCode, bytes_transferred);
        });
}

void UDPClient::HandleSentInput(Constants::InputTypes input, const boost::system::error_code& errorCode,
                                std::size_t bytes_transferred) {
    if (!errorCode) {
        cout << Utils::GetTime() << " - Ya se ha enviado el mensaje con input, " << input << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
        // ResendInput();
    }
}

void UDPClient::SendInputs(vector<Constants::InputTypes>& inputs) {
    boost::array<Constants::PetitionTypes, 1> petitionType = {Constants::PetitionTypes::SEND_INPUTS};
    boost::array<mutable_buffer, 2> outputBuffer = {
        boost::asio::buffer(petitionType),
        boost::asio::buffer(inputs)};
    socket.async_send_to(
        boost::asio::buffer(outputBuffer),
        serverEndpoint,
        boost::bind(
            &UDPClient::HandleSentInputs,
            this,
            inputs,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UDPClient::HandleSentInputs(const vector<Constants::InputTypes>& inputs, const boost::system::error_code& errorCode,
                                 std::size_t bytes_transferred) {
    if (!errorCode) {
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        cout << std::ctime(&end_time) << " - Ya se ha enviado el mensaje con " << inputs.size() << " inputs" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
        // ResendInput();
    }
}

void UDPClient::HandleSentDateTime(const boost::shared_ptr<std::string> message,
                                   const boost::system::error_code& errorCode,
                                   std::size_t bytes_transferred) {
    if (!errorCode) {
        cout << "Ya se ha enviado el mensaje, " << message << " madafaka" << endl;
    } else {
        cout << "Hubo un error enviando el mensaje, madafaka" << endl;
    }
}

void UDPClient::HandleReceived(const boost::system::error_code& errorCode, std::size_t bytesTransferred) {
    if (!errorCode) {
        cout << "Hemos recibido el mensaje " << recvBuff.data() << endl;
    } else {
        cout << "Hubo un error con código " << errorCode << endl;
    }
    StartReceiving();
}