namespace plb{

  inline PlbCaseTimer::PlbCaseTimer()
  {
    index_t global("global",GLOBAL_IND);
    indexMap.insert(global);
  
    clockVec.push_back(clock());
    timeVec.push_back(0);
    activeVec.push_back(false);
  }

  inline void PlbCaseTimer::registerCustom(char const *name_)
  {
    std::string name(name_);
  
    index_map_t::const_iterator it = indexMap.find(name);
  
    if(it != indexMap.end()){
      std::string errmsg("Error: custom timer ");
      errmsg.append(name);
      errmsg.append(" registered twice\n");
      throw PlbCaseTimerException(errmsg.c_str());
    }
  
    int index = clockVec.size();
    index_t ins(name,index);
    indexMap.insert(ins);
  
    clockVec.push_back(clock());
    timeVec.push_back(0.);
    activeVec.push_back(false);  
  }

  inline void PlbCaseTimer::startGlobal()
  {
    if(!global::mpi().isMainProcessor()) return;
    start(GLOBAL_IND);
  }

  inline void PlbCaseTimer::stopGlobal()
  {
    if(!global::mpi().isMainProcessor()) return;
    stop(GLOBAL_IND);
  }

  inline void PlbCaseTimer::startCustom(char const *name_)
  {
    if(!global::mpi().isMainProcessor()) return;
    int index = getIndex(name_);
    start(index);
  }

  inline void PlbCaseTimer::stopCustom(char const *name_)
  {
    if(!global::mpi().isMainProcessor()) return;
    int index = getIndex(name_);
    stop(index);
  }

  // get index for custom name

  inline int PlbCaseTimer::getIndex(char const *name_)
  {
    std::string name(name_);
  
    index_map_t::const_iterator it = indexMap.find(name);
  
    if(it == indexMap.end()){
      std::string errmsg("Error: custom timer ");
      errmsg.append(name);
      errmsg.append(" not found\n");
      throw PlbCaseTimerException(errmsg.c_str());
    }
  
    return it->second;
  }

  // generic start/stop functions that actually handle
  // the bookkeeping with clock_t

  inline void PlbCaseTimer::start(int const index)
  {
    if(activeVec[index]){
      std::string errmsg("Error: custom timer started twice");
      throw PlbCaseTimerException(errmsg.c_str());
    }
  
    clockVec[index] = clock();
    activeVec[index] = true;
  }
 
  inline void PlbCaseTimer::stop(int const index)
  {
    if(!activeVec[index]){
      std::string errmsg("Error: custom timer that should be stopped was not started before");
      throw PlbCaseTimerException(errmsg.c_str());
    }

    clock_t now = clock();

    timeVec[index] += now - clockVec[index];
    activeVec[index] = false;
  }

  inline double PlbCaseTimer::computeOther()
  {
    clock_t sum(0);
    for(pluint i=1;i<timeVec.size();i++)
      sum += timeVec[i];

    clock_t other = timeVec[0] - sum;
    return other/CPS;
  }

  inline void PlbCaseTimer::writeLog(Parallel_ostream &out)
  {
    for(index_map_t::iterator it = indexMap.begin();it!=indexMap.end();++it){
      out << "Timer " << it->first << " : " 
          << timeVec[it->second]/CPS << " seconds, " 
          << (double)timeVec[it->second]/(double)timeVec[GLOBAL_IND]*100 << "%" << std::endl;
    }

    double other = computeOther();
    out << "Timer other : " << other << " seconds, " 
        << other/(timeVec[GLOBAL_IND]/CPS) << "%" << std::endl;

  }

  inline void PlbCaseTimer::writeLogJSON(std::string fname)
  {
    fname.insert(0,global::directories().getOutputDir());

    plb_ofstream out(fname.c_str());
    
    out << "{\n";

    index_map_t::iterator it = indexMap.begin();
    for(index_map_t::iterator it = indexMap.begin();it!=indexMap.end();++it){
      out << "   \"" << it->first << "\":{\n";
      out << "      \"time\":" << timeVec[it->second]/CPS << ",\n";
      out << "      \"fraction\":" << (double)timeVec[it->second]/(double)timeVec[GLOBAL_IND] << "\n";
      out << "   },\n";
    }

    out << "   \"other\":{\n";
    out << "      \"time\":" << computeOther() << ",\n";
    out << "      \"fraction\":" << computeOther()/((double)timeVec[GLOBAL_IND]/CPS) << "\n";
    out << "   }\n";

    out << "}\n";

    out.close();
  }
}; // namespace plb
