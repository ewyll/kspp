#include <iostream>
#include <string>
#include <chrono>
#include <regex>
#include <kspp/codecs/text_codec.h>
#include <kspp/topology_builder.h>
#include <kspp/processors/transform.h>
#include <kspp/processors/count.h>
#include <kspp/algorithm.h>

#define PARTITION 0

int main(int argc, char **argv) {
  auto builder = kspp::topology_builder<kspp::text_codec>("example5-repartition", "localhost");
  {
    auto topology = builder.create_topic_topology();
    auto sink = topology->create_kafka_topic_sink<int, std::string>("kspp_example5_usernames");
    kspp::produce(*sink, 1, "user_1");
    kspp::produce(*sink, 2, "user_2");
    kspp::produce(*sink, 3, "user_3");
    kspp::produce(*sink, 4, "user_4");
    kspp::produce(*sink, 5, "user_5");
    kspp::produce(*sink, 6, "user_6");
    kspp::produce(*sink, 7, "user_7");
    kspp::produce(*sink, 8, "user_8");
    kspp::produce(*sink, 9, "user_9");
    kspp::produce(*sink, 10, "user_10");
  }

  {
    auto topology = builder.create_topic_topology();
    auto sink = topology->create_kafka_topic_sink<int, int>("kspp_example5_user_channel"); // <user_id, channel_id>
    kspp::produce(*sink, 1, 1);
    kspp::produce(*sink, 2, 1);
    kspp::produce(*sink, 3, 1);
    kspp::produce(*sink, 4, 1);
    kspp::produce(*sink, 5, 2);
    kspp::produce(*sink, 6, 2);
    kspp::produce(*sink, 7, 2);
    kspp::produce(*sink, 8, 2);
    kspp::produce(*sink, 9, 2);
    kspp::produce(*sink, 10, 2);
  }

  {
    auto topology = builder.create_topic_topology();
    auto sink = topology->create_kafka_topic_sink<int, std::string>("kspp_example5_channel_names");
    kspp::produce(*sink, 1, "channel1");
    kspp::produce(*sink, 2, "channel2");
  }

  {
    auto topology = builder.create_topic_topology(); 
    auto sources = topology->create_kafka_sources<int, std::string>("kspp_example5_usernames", 8);
    topology->create_stream_sink<int, std::string>(sources, std::cerr);
    
    for (auto s : sources) {
      std::cerr << s->name() << std::endl;
      s->start(-2);
      s->flush();
    }
  }

  {
    auto topology = builder.create_topic_topology();
    auto topic_sink = topology->create_kafka_topic_sink<int, std::string>("kspp_example5_usernames.per-channel");
    auto sources = topology->create_kafka_sources<int, std::string>("kspp_example5_usernames", 8);
    auto routing_sources = topology->create_kafka_sources<int, int>("kspp_example5_user_channel", 8);
    auto routing_tables = topology->create_ktables<int, int>(routing_sources);
    auto partition_repartition = topology->create_repartition<int, std::string, int>(sources, routing_tables, topic_sink);

    topology->init_metrics();
    topology->start(-2);
    topology->flush();
    topology->output_metrics(std::cerr);
  }


  {
    auto topology = builder.create_topic_topology();
    auto sources = topology->create_kafka_sources<int, std::string>("kspp_example5_usernames.per-channel", 8);
    topology->create_stream_sink<int, std::string>(sources, std::cerr);
    topology->start(-2);
    topology->flush();
    topology->output_metrics(std::cerr);
  }
}
