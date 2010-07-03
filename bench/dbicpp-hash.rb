#!/usr/bin/ruby

require_relative '../ruby/dbicpp'

DBI.init File.dirname(__FILE__) + '/../libs'

h = DBI::Handle.new user: 'udbicpp', db: 'dbicpp', driver: 'postgresql'

st = h.prepare "SELECT * FROM users"

(ARGV[0] || 10000).to_i.times {
  st.execute
  while r = st.next do
    p r
  end
}